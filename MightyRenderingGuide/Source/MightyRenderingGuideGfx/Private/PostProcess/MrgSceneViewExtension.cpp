#include "MrgSceneViewExtension.h"

#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "MrgPostProcessShaders.h"
#include "PostProcess/PostProcessInputs.h"

FMrgSceneViewExtension::FMrgSceneViewExtension(const FAutoRegister& AutoRegister)
	: FSceneViewExtensionBase(AutoRegister)
{
}

void FMrgSceneViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View,
                                                             const FPostProcessingInputs& Inputs)
{
	Inputs.Validate();

	if (!ShouldRender(View))
	{
		return;
	}

	FSceneViewExtensionBase::PrePostProcessPass_RenderThread(GraphBuilder, View, Inputs);

	FRDGTexture* GBufferTexture = (*Inputs.SceneTextures)->GBufferBTexture;

	TShaderMapRef<FMrgShadePostProcessShaderCS> Shader(GetGlobalShaderMap(View.GetFeatureLevel()));

	FMrgShadePostProcessShaderCS::FParameters* Params = GraphBuilder.AllocParameters<
		FMrgShadePostProcessShaderCS::FParameters>();
	Params->SceneTextureSize = FIntPoint(View.UnscaledViewRect.Width(), View.UnscaledViewRect.Height());
	Params->SceneTexture = GraphBuilder.CreateUAV((*Inputs.SceneTextures)->SceneColorTexture);
	Params->GBuffer = GraphBuilder.CreateSRV(GBufferTexture);
	Params->GBufferExtent = FVector2f(GBufferTexture->Desc.Extent);
	Params->GBufferInverseExtent = FVector2f(1.0f) / Params->GBufferExtent;
	Params->SourceSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();
	Params->EdgeColor = FVector4f(0.006995f, 0.008023f, 0.021219f, 1.0f);
	Params->EdgeStrength = 10000.0f;

	const FIntVector ThreadGroupCount = FComputeShaderUtils::GetGroupCount(
		FIntVector(Params->SceneTextureSize.X, Params->SceneTextureSize.Y, 1),
		FIntVector(8, 8, 1));

	FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("Outline"), Shader, Params, ThreadGroupCount);
}

bool FMrgSceneViewExtension::ShouldRender(const FSceneView& View)
{
	return View.bIsGameView && !View.bIsSceneCapture;
}
