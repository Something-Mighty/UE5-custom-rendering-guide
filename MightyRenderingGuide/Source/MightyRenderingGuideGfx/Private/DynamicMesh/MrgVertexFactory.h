#pragma once

#include "CoreMinimal.h"
#include "DataDrivenShaderPlatformInfo.h"
#include "MeshMaterialShader.h"

struct FMrgMeshBatchElementUserData
{
	float WaveAmplitude;
	float WaveFrequency;
};

struct FMrgMeshBatchElementOneFrameResource final : FOneFrameResource
{
	FMrgMeshBatchElementUserData Payload;
};

class FMrgVertexFactoryShaderParameters : public FVertexFactoryShaderParameters
{
	DECLARE_TYPE_LAYOUT(FMrgVertexFactoryShaderParameters, NonVirtual);

public:
	void Bind(const FShaderParameterMap& ParameterMap);
	void GetElementShaderBindings(const class FSceneInterface* Scene, const FSceneView* View,
	                              const FMeshMaterialShader* Shader, const EVertexInputStreamType InputStreamType,
	                              ERHIFeatureLevel::Type FeatureLevel,
	                              const FVertexFactory* VertexFactory, const FMeshBatchElement& BatchElement,
	                              class FMeshDrawSingleShaderBindings& ShaderBindings,
	                              FVertexInputStreamArray& VertexStreams) const;

	LAYOUT_FIELD(FShaderParameter, WaveAmplitude);
	LAYOUT_FIELD(FShaderParameter, WaveFrequency);
};

class FMrgVertexFactory : public FVertexFactory
{
	DECLARE_VERTEX_FACTORY_TYPE(FMrgVertexFactory);

public:
	FMrgVertexFactory(const ERHIFeatureLevel::Type InFeatureLevel);
	virtual ~FMrgVertexFactory() override = default;

	static bool ShouldCache(const FVertexFactoryShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	static bool ShouldCompilePermutation(const FVertexFactoryShaderPermutationParameters& Parameters);

	virtual void InitRHI(FRHICommandListBase& RHICmdList) override;

	void SetData(FRHICommandListBase& RHICmdList, const FStaticMeshDataType& InData);

private:
	FStaticMeshDataType Data;
};
