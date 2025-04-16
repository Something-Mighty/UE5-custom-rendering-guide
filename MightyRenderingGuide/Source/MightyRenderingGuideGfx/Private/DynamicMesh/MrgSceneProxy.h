#pragma once

#include "CoreMinimal.h"
#include "MrgVertexFactory.h"
#include "DynamicMeshBuilder.h"
#include "UObject/Object.h"

class UMrgDynamicMeshComponent;

class FMrgIndexBuffer final : public FIndexBuffer
{
public:
	FMrgIndexBuffer() = default;

	virtual void InitRHI(FRHICommandListBase& RHICmdList) override;

	// private:
	TArray<int32> Indices;
};

class FMrgSceneProxy : public FPrimitiveSceneProxy
{
public:
	explicit FMrgSceneProxy(const UMrgDynamicMeshComponent* InComponent);
	virtual ~FMrgSceneProxy() override;

	virtual SIZE_T GetTypeHash() const override;
	virtual uint32 GetMemoryFootprint() const override;

	virtual void CreateRenderThreadResources(FRHICommandListBase& RHICmdList) override;

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily,
	                                    uint32 VisibilityMap, FMeshElementCollector& Collector) const override;

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;

	uint32 GetNumTriangles() const;
	uint32 GetNumVertices() const;

private:
	void BuildCenterMesh();

	const UMrgDynamicMeshComponent* Component = nullptr;
	TObjectPtr<UMaterialInterface> Material;


	FStaticMeshVertexBuffers VertexBuffers;
	FMrgIndexBuffer IndexBuffer;

	FMrgVertexFactory VertexFactory;
	TArray<FDynamicMeshVertex> Vertices;
	TArray<int32> Indices;
};
