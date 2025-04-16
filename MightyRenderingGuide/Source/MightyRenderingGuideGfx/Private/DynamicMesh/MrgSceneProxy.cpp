#include "MrgSceneProxy.h"
#include "MrgDynamicMeshComponent.h"
#include "MaterialDomain.h"
#include "Materials/MaterialRenderProxy.h"

void FMrgIndexBuffer::InitRHI(FRHICommandListBase& RHICmdList)
{
	FRHIResourceCreateInfo CreateInfo(TEXT("FMrgIndexBuffer"));
	const uint32 DataSize = Indices.Num() * sizeof(int32);
	SetRHI(RHICmdList.CreateIndexBuffer(sizeof(int32), DataSize, BUF_Static, CreateInfo));
	void* BufferMemory = RHICmdList.LockBuffer(GetRHI(), 0, DataSize, RLM_WriteOnly);
	FMemory::Memcpy(BufferMemory, Indices.GetData(), DataSize);
	RHICmdList.UnlockBuffer(GetRHI());
}

FMrgSceneProxy::FMrgSceneProxy(const UMrgDynamicMeshComponent* InComponent)
	: FPrimitiveSceneProxy(InComponent)
	  , Component(InComponent)
	  , VertexFactory(GetScene().GetFeatureLevel())
{
	SetWireframeColor(FLinearColor::Yellow);
	BuildCenterMesh();

	Material = Component->Material ? Component->Material : UMaterial::GetDefaultMaterial(MD_Surface);
}

FMrgSceneProxy::~FMrgSceneProxy()
{
	VertexBuffers.PositionVertexBuffer.ReleaseResource();
	VertexBuffers.StaticMeshVertexBuffer.ReleaseResource();
	VertexBuffers.ColorVertexBuffer.ReleaseResource();
	IndexBuffer.ReleaseResource();
	VertexFactory.ReleaseResource();
}

SIZE_T FMrgSceneProxy::GetTypeHash() const
{
	static size_t UniquePointer;
	return reinterpret_cast<size_t>(&UniquePointer);
}

uint32 FMrgSceneProxy::GetMemoryFootprint() const
{
	return sizeof(*this) + GetAllocatedSize();
}

static void InitOrUpdateResource(FRHICommandListBase& RHICmdList, FRenderResource* Resource)
{
	if (!Resource->IsInitialized())
	{
		Resource->InitResource(RHICmdList);
	}
	else
	{
		Resource->UpdateRHI(RHICmdList);
	}
}

void FMrgSceneProxy::CreateRenderThreadResources(FRHICommandListBase& RHICmdList)
{
	const FName Name = FName(TEXT("FMrgSceneProxy ") + GetOwnerName().ToString());

	VertexBuffers.SetOwnerName(Name);
	{
		VertexBuffers.PositionVertexBuffer.Init(Vertices.Num());
		VertexBuffers.StaticMeshVertexBuffer.Init(Vertices.Num(), 2);
		VertexBuffers.ColorVertexBuffer.Init(Vertices.Num());

		for (int32 i = 0; i < Vertices.Num(); i++)
		{
			const FDynamicMeshVertex& Vertex = Vertices[i];

			VertexBuffers.PositionVertexBuffer.VertexPosition(i) = Vertex.Position;
			VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(i, Vertex.TangentX.ToFVector3f(),
			                                                       Vertex.GetTangentY(), Vertex.TangentZ.ToFVector3f());
			VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(i, 0, Vertex.TextureCoordinate[0]);
			VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(i, 1, Vertex.TextureCoordinate[1]);
			VertexBuffers.ColorVertexBuffer.VertexColor(i) = Vertex.Color;
		}

		InitOrUpdateResource(RHICmdList, &VertexBuffers.PositionVertexBuffer);
		InitOrUpdateResource(RHICmdList, &VertexBuffers.StaticMeshVertexBuffer);
		InitOrUpdateResource(RHICmdList, &VertexBuffers.ColorVertexBuffer);

		FStaticMeshDataType Data;
		VertexBuffers.PositionVertexBuffer.BindPositionVertexBuffer(&VertexFactory, Data);
		VertexBuffers.StaticMeshVertexBuffer.BindTangentVertexBuffer(&VertexFactory, Data);
		VertexBuffers.StaticMeshVertexBuffer.BindTexCoordVertexBuffer(&VertexFactory, Data);
		VertexBuffers.ColorVertexBuffer.BindColorVertexBuffer(&VertexFactory, Data);
		VertexFactory.SetData(RHICmdList, Data);

		InitOrUpdateResource(RHICmdList, &VertexFactory);
	}


	IndexBuffer.SetOwnerName(Name);
	IndexBuffer.Indices = Indices;
	IndexBuffer.InitResource(RHICmdList);
}

void FMrgSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views,
                                            const FSceneViewFamily& ViewFamily, uint32 VisibilityMap,
                                            FMeshElementCollector& Collector) const
{
	const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

	FColoredMaterialRenderProxy* WireframeMaterialInstance = nullptr;
	if (bWireframe)
	{
		WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy() : nullptr,
			FColor::Cyan);
		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);
	}

	const FMaterialRenderProxy* const MaterialProxy = (bWireframe && WireframeMaterialInstance != nullptr)
		                                                  ? WireframeMaterialInstance
		                                                  : Material->GetRenderProxy();

	check(MaterialProxy != nullptr);

	for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
	{
		if (VisibilityMap & (1 << ViewIndex))
		{
			FMeshBatch& Mesh = Collector.AllocateMesh();
			Mesh.VertexFactory = &VertexFactory;
			Mesh.MaterialRenderProxy = MaterialProxy;
			Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
			Mesh.Type = PT_TriangleList;
			Mesh.DepthPriorityGroup = SDPG_World;
			Mesh.MeshIdInPrimitive = 0;
			Mesh.LODIndex = 0;
			Mesh.bWireframe = bWireframe;
			Mesh.SegmentIndex = 0;
			Mesh.bDisableBackfaceCulling = true;

			FMeshBatchElement& BatchElement = Mesh.Elements[0];
			BatchElement.IndexBuffer = &IndexBuffer;
			BatchElement.PrimitiveUniformBuffer = GetUniformBuffer();
			BatchElement.FirstIndex = 0;
			BatchElement.NumPrimitives = GetNumTriangles();
			BatchElement.MinVertexIndex = 0;
			BatchElement.MaxVertexIndex = GetNumVertices() - 1;

			FMrgMeshBatchElementOneFrameResource* UserData = &Collector.AllocateOneFrameResource<
				FMrgMeshBatchElementOneFrameResource>();
			UserData->Payload.WaveAmplitude = Component->WaveAmplitude;
			UserData->Payload.WaveFrequency = Component->WaveFrequency;
			BatchElement.UserData = (void*)UserData;

			Collector.AddMesh(ViewIndex, Mesh);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			RenderBounds(Collector.GetPDI(ViewIndex), ViewFamily.EngineShowFlags, GetBounds(), IsSelected());
#endif
		}
	}
}

FPrimitiveViewRelevance FMrgSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	FPrimitiveViewRelevance Result;

	Result.bDrawRelevance = IsShown(View);
	Result.bShadowRelevance = IsShadowCast(View);

	Result.bStaticRelevance = false;
	Result.bDynamicRelevance = true;

	Result.bRenderInMainPass = ShouldRenderInMainPass();
	Result.bRenderInDepthPass = ShouldRenderInDepthPass();
	Result.bRenderCustomDepth = ShouldRenderCustomDepth();
	Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
	Result.bVelocityRelevance = DrawsVelocity() && Result.bOpaque && Result.bRenderInMainPass;

#if WITH_EDITOR
	Result.bEditorStaticSelectionRelevance = (IsSelected() || IsHovered());
	Result.bEditorVisualizeLevelInstanceRelevance = IsEditingLevelInstanceChild();
#endif

	return Result;
}

uint32 FMrgSceneProxy::GetNumTriangles() const
{
	return Indices.Num() / 3;
}

uint32 FMrgSceneProxy::GetNumVertices() const
{
	return Vertices.Num() - 1;
}

void FMrgSceneProxy::BuildCenterMesh()
{
	const uint16 VertexCount = Component->VertexCount;
	const float Radius = Component->Radius;

	Vertices.Reserve(VertexCount + 1);
	Indices.Reserve(VertexCount * 3);

	// Add the center vertex
	{
		FDynamicMeshVertex Vert(FVector3f(0.0f, 0.0f, 0.0f), FVector2f(0.0f, 0.0f), FColor::Black);
		Vert.TextureCoordinate[1] = FVector2f(-1.0f, 0.0f);
		Vertices.Add(Vert);
	}

	for (uint16 i = 0; i < VertexCount; i++)
	{
		// Compute the angle for this vertex
		const float Angle = 2.0f * PI * i / VertexCount;

		// Compute the position of the vertex on the circle's perimeter
		const float X = Radius * FMath::Cos(Angle);
		const float Y = Radius * FMath::Sin(Angle);

		// Create and add the vertex
		FDynamicMeshVertex Vert(FVector3f(X, Y, 0.0f), FVector2f(X / Radius, Y / Radius), FColor::Black);
		Vert.TextureCoordinate[1] = FVector2f(-1.0f, 0.0f);
		Vertices.Add(Vert);

		// Add indices for the triangle formed with the center vertex (index 0)
		if (i > 0)
		{
			Indices.Add(0); // Center vertex
			Indices.Add(i); // Current vertex
			Indices.Add(i + 1); // Next vertex (wrapping back to 1)
		}

		// Handle wrapping the last triangle to the first vertex on the perimeter
		if (i == VertexCount - 1)
		{
			Indices.Add(0); // Center vertex
			Indices.Add(i + 1); // Last vertex
			Indices.Add(1); // First vertex
		}
	}


	// Add vertices for the wave border
	for (uint16 i = 0; i < VertexCount; i++)
	{
		// Compute the angle for this vertex
		const float Angle = 2.0f * PI * i / VertexCount;

		// Compute wave displacement
		// const float Displacement = (WaveAmplitude * 1.5f) + WaveAmplitude * FMath::Sin(WaveFrequency * Angle);
		const float Displacement = 0.0f;

		// Compute the position of the vertex on the wave border
		const float X = (Radius + Displacement) * FMath::Cos(Angle);
		const float Y = (Radius + Displacement) * FMath::Sin(Angle);

		// Create and add the vertex
		FDynamicMeshVertex Vert(FVector3f(X, Y, 0.0f), FVector2f((X / Radius) * 1.5f, (Y / Radius) * 1.5f),
		                        FColor::Blue);
		Vert.TextureCoordinate[1] = FVector2f(Angle, 0.0f);
		const uint32 NewVertex = Vertices.Add(Vert);

		// Add indices for the new triangles connecting the original perimeter and the waved border
		if (i != VertexCount - 1)
		{
			Indices.Add(i + 1);
			Indices.Add(NewVertex);
			Indices.Add(NewVertex + 1);

			Indices.Add(i + 1);
			Indices.Add(NewVertex + 1);
			Indices.Add(i + 2);
		}
		else
		{
			Indices.Add(i + 1);
			Indices.Add(NewVertex);
			Indices.Add(VertexCount + 1);

			Indices.Add(i + 1);
			Indices.Add(VertexCount + 1);
			Indices.Add(1);
		}
	}
}
