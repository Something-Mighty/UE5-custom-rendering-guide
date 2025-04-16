#pragma once
#include "ShaderParameterStruct.h"

class FMrgShadePostProcessShaderCS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FMrgShadePostProcessShaderCS);
	SHADER_USE_PARAMETER_STRUCT(FMrgShadePostProcessShaderCS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
		SHADER_PARAMETER(FIntPoint, SceneTextureSize)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, SceneTexture)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, GBuffer)
		SHADER_PARAMETER(FVector2f, GBufferExtent)
		SHADER_PARAMETER(FVector2f, GBufferInverseExtent)
		SHADER_PARAMETER_SAMPLER(SamplerState, SourceSampler)
		SHADER_PARAMETER(FVector4f, EdgeColor)
		SHADER_PARAMETER(float, EdgeStrength)
	END_SHADER_PARAMETER_STRUCT()
};
