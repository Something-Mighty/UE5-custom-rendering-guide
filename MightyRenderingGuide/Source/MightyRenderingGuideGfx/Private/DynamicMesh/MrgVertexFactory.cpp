#include "MrgVertexFactory.h"

#include "MaterialDomain.h"
#include "MeshDrawShaderBindings.h"

FMrgVertexFactory::FMrgVertexFactory(const ERHIFeatureLevel::Type InFeatureLevel)
	: FVertexFactory(InFeatureLevel)
{
}

bool FMrgVertexFactory::ShouldCompilePermutation(const FVertexFactoryShaderPermutationParameters& Parameters)
{
	return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5) && Parameters.MaterialParameters.
		MaterialDomain == MD_Surface;
}

void FMrgVertexFactory::InitRHI(FRHICommandListBase& RHICmdList)
{
	FVertexDeclarationElementList Elements;

	Elements.Add(AccessStreamComponent(Data.PositionComponent, 0, Streams));
	Elements.Add(AccessStreamComponent(Data.TangentBasisComponents[0], 1, Streams));
	Elements.Add(AccessStreamComponent(Data.TangentBasisComponents[1], 2, Streams));
	Elements.Add(AccessStreamComponent(Data.ColorComponent, 3, Streams));
	Elements.Add(AccessStreamComponent(Data.TextureCoordinates[0], 4, Streams));
	Elements.Add(AccessStreamComponent(Data.TextureCoordinates[1], 5, Streams));

	InitDeclaration(Elements);
}

void FMrgVertexFactory::SetData(FRHICommandListBase& RHICmdList, const FStaticMeshDataType& InData)
{
	Data = InData;
	UpdateRHI(RHICmdList);
}

void FMrgVertexFactoryShaderParameters::Bind(const FShaderParameterMap& ParameterMap)
{
	WaveAmplitude.Bind(ParameterMap, TEXT("WaveAmplitude"));
	WaveFrequency.Bind(ParameterMap, TEXT("WaveFrequency"));
}

void FMrgVertexFactoryShaderParameters::GetElementShaderBindings(const FSceneInterface* Scene,
                                                                 const FSceneView* View,
                                                                 const FMeshMaterialShader* Shader,
                                                                 const EVertexInputStreamType InputStreamType,
                                                                 ERHIFeatureLevel::Type FeatureLevel,
                                                                 const FVertexFactory* VertexFactory,
                                                                 const FMeshBatchElement& BatchElement,
                                                                 FMeshDrawSingleShaderBindings&
                                                                 ShaderBindings,
                                                                 FVertexInputStreamArray& VertexStreams) const
{
	const FMrgMeshBatchElementOneFrameResource* UserData = (FMrgMeshBatchElementOneFrameResource*)BatchElement.UserData;
	ShaderBindings.Add(WaveAmplitude, UserData->Payload.WaveAmplitude);
	ShaderBindings.Add(WaveFrequency, UserData->Payload.WaveFrequency);
}

IMPLEMENT_TYPE_LAYOUT(FMrgVertexFactoryShaderParameters);

IMPLEMENT_VERTEX_FACTORY_PARAMETER_TYPE(FMrgVertexFactory, SF_Vertex, FMrgVertexFactoryShaderParameters);

IMPLEMENT_VERTEX_FACTORY_TYPE(FMrgVertexFactory, "/MRG/Shader/MRGVertexFactory.ush",
                              EVertexFactoryFlags::UsedWithMaterials
                              | EVertexFactoryFlags::SupportsPositionOnly
);
