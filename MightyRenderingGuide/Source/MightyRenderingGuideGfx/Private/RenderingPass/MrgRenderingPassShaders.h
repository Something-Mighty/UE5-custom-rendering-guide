#pragma once
#include "ScreenPass.h"
#include "ShaderParameterStruct.h"

// Linearize Shader
class FMrgLinearizeShaderPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FMrgLinearizeShaderPS);
	SHADER_USE_PARAMETER_STRUCT(FMrgLinearizeShaderPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
		RENDER_TARGET_BINDING_SLOTS()

		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Input)
		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Output)

		SHADER_PARAMETER(float, GAMMA_INPUT)

		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, Source)
		SHADER_PARAMETER_SAMPLER(SamplerState, SourceSampler)
	END_SHADER_PARAMETER_STRUCT()
};

// Blur Horizontal shader
class FMrgBlurHorizontalShaderPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FMrgBlurHorizontalShaderPS);
	SHADER_USE_PARAMETER_STRUCT(FMrgBlurHorizontalShaderPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
		RENDER_TARGET_BINDING_SLOTS()

		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Input)
		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Output)

		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, Source)
		SHADER_PARAMETER_SAMPLER(SamplerState, SourceSampler)
	END_SHADER_PARAMETER_STRUCT()
};


// Blur Vertical shader
class FMrgBlurVerticalShaderPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FMrgBlurVerticalShaderPS);
	SHADER_USE_PARAMETER_STRUCT(FMrgBlurVerticalShaderPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
		RENDER_TARGET_BINDING_SLOTS()

		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Input)
		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Output)

		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, Source)
		SHADER_PARAMETER_SAMPLER(SamplerState, SourceSampler)
	END_SHADER_PARAMETER_STRUCT()
};


// Threshold shader
class FMrgThresholdShaderPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FMrgThresholdShaderPS);
	SHADER_USE_PARAMETER_STRUCT(FMrgThresholdShaderPS, FGlobalShader)

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
		RENDER_TARGET_BINDING_SLOTS()

		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Input)
		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Output)

		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, Source)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, ORIG_LINEARIZED)
		SHADER_PARAMETER_SAMPLER(SamplerState, Sampler)
	END_SHADER_PARAMETER_STRUCT()
};

// Bloom shader
class FMrgBloomShaderPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FMrgBloomShaderPS);
	SHADER_USE_PARAMETER_STRUCT(FMrgBloomShaderPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
		RENDER_TARGET_BINDING_SLOTS()

		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Input)
		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Output)

		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, Source)
		SHADER_PARAMETER_SAMPLER(SamplerState, SourceSampler)

		SHADER_PARAMETER(float, hardScan)
		SHADER_PARAMETER(float, hardPix)
		SHADER_PARAMETER(float, warpX)
		SHADER_PARAMETER(float, warpY)
		SHADER_PARAMETER(float, maskDark)
		SHADER_PARAMETER(float, maskLight)
		SHADER_PARAMETER(float, scaleInLinearGamma)
		SHADER_PARAMETER(float, shadowMask)
		SHADER_PARAMETER(float, brightBoost)
		SHADER_PARAMETER(float, hardBloomScan)
		SHADER_PARAMETER(float, hardBloomPix)
		SHADER_PARAMETER(float, bloomAmount)
		SHADER_PARAMETER(float, shape)
	END_SHADER_PARAMETER_STRUCT()
};

// Scanline shader
class FMrgScanlineShaderPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FMrgScanlineShaderPS);
	SHADER_USE_PARAMETER_STRUCT(FMrgScanlineShaderPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
		RENDER_TARGET_BINDING_SLOTS()

		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Input)
		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, Output)

		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, Source)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, ORIG_LINEARIZED)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, BloomPass)
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, GlowPass)
		SHADER_PARAMETER_SAMPLER(SamplerState, SourceSampler)

		SHADER_PARAMETER(float, hardScan)
		SHADER_PARAMETER(float, hardPix)
		SHADER_PARAMETER(float, warpX)
		SHADER_PARAMETER(float, warpY)
		SHADER_PARAMETER(float, maskDark)
		SHADER_PARAMETER(float, maskLight)
		SHADER_PARAMETER(float, scaleInLinearGamma)
		SHADER_PARAMETER(float, shadowMask)
		SHADER_PARAMETER(float, brightBoost)
		SHADER_PARAMETER(float, hardBloomScan)
		SHADER_PARAMETER(float, hardBloomPix)
		SHADER_PARAMETER(float, bloomAmount)
		SHADER_PARAMETER(float, shape)
		SHADER_PARAMETER(float, DIFFUSION)

	END_SHADER_PARAMETER_STRUCT()
};
