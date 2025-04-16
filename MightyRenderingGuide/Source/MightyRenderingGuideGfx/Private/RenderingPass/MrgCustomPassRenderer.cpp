#include "MrgCustomPassRenderer.h"

#include "MrgRenderingPassShaders.h"
#include "PixelShaderUtils.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphEvent.h"
#include "Engine/TextureRenderTarget2D.h"

UMrgCustomPassRenderer::UMrgCustomPassRenderer()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMrgCustomPassRenderer::Render()
{
	check(Source);

	const ERHIFeatureLevel::Type FeatureLevel = GetWorld()->GetFeatureLevel();
	FTextureRenderTargetResource* SourceResource = Source->GameThread_GetRenderTargetResource();
	FTextureRenderTargetResource* TargetResource = Target->GameThread_GetRenderTargetResource();

	ENQUEUE_RENDER_COMMAND(MRG_CRT_RenderCommands)
	(
		[FeatureLevel, SourceResource, TargetResource, CRTShaderParameters = this->ShaderParameters](
		FRHICommandListImmediate& RHICmdList)
		{
			FRDGBuilder GraphBuilder(RHICmdList, RDG_EVENT_NAME("MRG Render"));

			FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(FeatureLevel);

			// Define the resources to RDG
			FRDGTextureRef RDG_Source = GraphBuilder.RegisterExternalTexture(
				CreateRenderTarget(SourceResource->GetTextureRHI(), TEXT("MRG.Source")));
			FRDGTextureRef RDG_Target = GraphBuilder.RegisterExternalTexture(
				CreateRenderTarget(TargetResource->GetTextureRHI(), TEXT("MRG.Target")));

			// Define the internal resources

			FRDGTextureRef RDG_Linearized = GraphBuilder.CreateTexture(RDG_Source->Desc, TEXT("MRG.Linearized"));

			FRDGTextureRef RDG_BlurHorizontal = GraphBuilder.CreateTexture(
				RDG_Source->Desc, TEXT("MRG.BlurHorizontal"));
			FRDGTextureRef RDG_BlurVertical = GraphBuilder.
				CreateTexture(RDG_Source->Desc, TEXT("MRG.BlurVertical"));
			FRDGTextureRef RDG_Threshold = GraphBuilder.CreateTexture(RDG_Source->Desc, TEXT("MRG.Threshold"));
			FRDGTextureRef RDG_Bloom = GraphBuilder.CreateTexture(RDG_Source->Desc, TEXT("MRG.Bloom"));

			const FIntRect Viewport(0, 0, TargetResource->GetSizeX(), TargetResource->GetSizeY());

			// Linearize
			{
				TShaderMapRef<FMrgLinearizeShaderPS> LinearizeShader(GlobalShaderMap);

				FMrgLinearizeShaderPS::FParameters* Parameters = GraphBuilder.AllocParameters<
					FMrgLinearizeShaderPS::FParameters>();

				Parameters->RenderTargets[0] = FRenderTargetBinding(RDG_Linearized, ERenderTargetLoadAction::ELoad);
				Parameters->Input = GetScreenPassTextureViewportParameters(FScreenPassTextureViewport(RDG_Source));
				Parameters->Output = GetScreenPassTextureViewportParameters(FScreenPassTextureViewport(RDG_Linearized));
				Parameters->Source = GraphBuilder.CreateSRV(RDG_Source);
				Parameters->SourceSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();
				Parameters->GAMMA_INPUT = 2.4f;

				FPixelShaderUtils::AddFullscreenPass(GraphBuilder, GlobalShaderMap, RDG_EVENT_NAME("Linearize"),
				                                     LinearizeShader, Parameters, Viewport,
				                                     TStaticBlendState<
					                                     CW_RGBA, BO_Add, BF_SourceAlpha,
					                                     BF_InverseSourceAlpha>::GetRHI());
			}

			// Blur horizontal
			{
				TShaderMapRef<FMrgBlurHorizontalShaderPS> BlurHorizontalShader(GlobalShaderMap);
				FMrgBlurHorizontalShaderPS::FParameters* Parameters = GraphBuilder.AllocParameters<
					FMrgBlurHorizontalShaderPS::FParameters>();

				Parameters->RenderTargets[0] = FRenderTargetBinding(RDG_BlurHorizontal, ERenderTargetLoadAction::ELoad);
				Parameters->Input = GetScreenPassTextureViewportParameters(FScreenPassTextureViewport(RDG_Linearized));
				Parameters->Output = GetScreenPassTextureViewportParameters(
					FScreenPassTextureViewport(RDG_BlurHorizontal));
				Parameters->Source = GraphBuilder.CreateSRV(RDG_Linearized);
				Parameters->SourceSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();

				FPixelShaderUtils::AddFullscreenPass(GraphBuilder, GlobalShaderMap, RDG_EVENT_NAME("Blur Horizontal"),
				                                     BlurHorizontalShader, Parameters, Viewport,
				                                     TStaticBlendState<
					                                     CW_RGBA, BO_Add, BF_SourceAlpha,
					                                     BF_InverseSourceAlpha>::GetRHI());
			}

			// Blur vertical
			{
				TShaderMapRef<FMrgBlurVerticalShaderPS> BlurVerticalShader(GlobalShaderMap);
				FMrgBlurVerticalShaderPS::FParameters* Parameters = GraphBuilder.AllocParameters<
					FMrgBlurVerticalShaderPS::FParameters>();

				Parameters->RenderTargets[0] = FRenderTargetBinding(RDG_BlurVertical, ERenderTargetLoadAction::ELoad);
				Parameters->Input = GetScreenPassTextureViewportParameters(
					FScreenPassTextureViewport(RDG_BlurHorizontal));
				Parameters->Output = GetScreenPassTextureViewportParameters(
					FScreenPassTextureViewport(RDG_BlurVertical));
				Parameters->Source = GraphBuilder.CreateSRV(RDG_BlurHorizontal);
				Parameters->SourceSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();

				FPixelShaderUtils::AddFullscreenPass(GraphBuilder, GlobalShaderMap, RDG_EVENT_NAME("Blur Vertical"),
				                                     BlurVerticalShader, Parameters, Viewport,
				                                     TStaticBlendState<
					                                     CW_RGBA, BO_Add, BF_SourceAlpha,
					                                     BF_InverseSourceAlpha>::GetRHI());
			}

			// Threshold
			{
				TShaderMapRef<FMrgThresholdShaderPS> ThresholdShader(GlobalShaderMap);
				FMrgThresholdShaderPS::FParameters* Parameters = GraphBuilder.AllocParameters<
					FMrgThresholdShaderPS::FParameters>();

				Parameters->RenderTargets[0] = FRenderTargetBinding(RDG_Threshold, ERenderTargetLoadAction::ELoad);
				Parameters->Input =
					GetScreenPassTextureViewportParameters(FScreenPassTextureViewport(RDG_BlurVertical));
				Parameters->Output = GetScreenPassTextureViewportParameters(FScreenPassTextureViewport(RDG_Threshold));
				Parameters->Source = GraphBuilder.CreateSRV(RDG_BlurVertical);
				Parameters->ORIG_LINEARIZED = GraphBuilder.CreateSRV(RDG_Linearized);
				Parameters->Sampler = TStaticSamplerState<SF_Bilinear>::GetRHI();

				FPixelShaderUtils::AddFullscreenPass(GraphBuilder, GlobalShaderMap, RDG_EVENT_NAME("Threshold"),
				                                     ThresholdShader, Parameters, Viewport,
				                                     TStaticBlendState<
					                                     CW_RGBA, BO_Add, BF_SourceAlpha,
					                                     BF_InverseSourceAlpha>::GetRHI());
			}

			// Bloom
			{
				TShaderMapRef<FMrgBloomShaderPS> BloomShader(GlobalShaderMap);
				FMrgBloomShaderPS::FParameters* Parameters = GraphBuilder.AllocParameters<
					FMrgBloomShaderPS::FParameters>();

				Parameters->RenderTargets[0] = FRenderTargetBinding(RDG_Bloom, ERenderTargetLoadAction::ELoad);
				Parameters->Input = GetScreenPassTextureViewportParameters(FScreenPassTextureViewport(RDG_Threshold));
				Parameters->Output = GetScreenPassTextureViewportParameters(FScreenPassTextureViewport(RDG_Source));
				Parameters->Source = GraphBuilder.CreateSRV(RDG_Threshold);
				Parameters->SourceSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();

				Parameters->hardScan = CRTShaderParameters.HardScan;
				Parameters->hardPix = CRTShaderParameters.HardPix;
				Parameters->warpX = CRTShaderParameters.WarpX;
				Parameters->warpY = CRTShaderParameters.WarpY;
				Parameters->maskDark = CRTShaderParameters.MaskDark;
				Parameters->maskLight = CRTShaderParameters.MaskLight;
				Parameters->scaleInLinearGamma = CRTShaderParameters.ScaleInLinearGamma;
				Parameters->shadowMask = CRTShaderParameters.ShadowMask;
				Parameters->brightBoost = CRTShaderParameters.BrightBoost;
				Parameters->hardBloomPix = CRTShaderParameters.HardBloomPix;
				Parameters->hardBloomScan = CRTShaderParameters.HardBloomScan;
				Parameters->bloomAmount = CRTShaderParameters.BloomAmount;
				Parameters->shape = CRTShaderParameters.Shape;

				FPixelShaderUtils::AddFullscreenPass(GraphBuilder, GlobalShaderMap, RDG_EVENT_NAME("Bloom"),
				                                     BloomShader, Parameters, Viewport,
				                                     TStaticBlendState<
					                                     CW_RGBA, BO_Add, BF_SourceAlpha,
					                                     BF_InverseSourceAlpha>::GetRHI());
			}

			// Scanline
			{
				TShaderMapRef<FMrgScanlineShaderPS> ScanlineShader(GlobalShaderMap);
				FMrgScanlineShaderPS::FParameters* Parameters = GraphBuilder.AllocParameters<
					FMrgScanlineShaderPS::FParameters>();

				Parameters->RenderTargets[0] = FRenderTargetBinding(RDG_Target, ERenderTargetLoadAction::ELoad);
				Parameters->Input = GetScreenPassTextureViewportParameters(FScreenPassTextureViewport(RDG_Source));
				Parameters->Source = GraphBuilder.CreateSRV(RDG_Bloom);
				Parameters->ORIG_LINEARIZED = GraphBuilder.CreateSRV(RDG_Linearized);
				Parameters->BloomPass = GraphBuilder.CreateSRV(RDG_Bloom);
				Parameters->GlowPass = GraphBuilder.CreateSRV(RDG_Threshold);
				Parameters->Output = GetScreenPassTextureViewportParameters(FScreenPassTextureViewport(RDG_Target));
				Parameters->SourceSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();

				Parameters->hardScan = CRTShaderParameters.HardScan;
				Parameters->hardPix = CRTShaderParameters.HardPix;
				Parameters->warpX = CRTShaderParameters.WarpX;
				Parameters->warpY = CRTShaderParameters.WarpY;
				Parameters->maskDark = CRTShaderParameters.MaskDark;
				Parameters->maskLight = CRTShaderParameters.MaskLight;
				Parameters->scaleInLinearGamma = CRTShaderParameters.ScaleInLinearGamma;
				Parameters->shadowMask = CRTShaderParameters.ShadowMask;
				Parameters->brightBoost = CRTShaderParameters.BrightBoost;
				Parameters->hardBloomPix = CRTShaderParameters.HardBloomPix;
				Parameters->hardBloomScan = CRTShaderParameters.HardBloomScan;
				Parameters->bloomAmount = CRTShaderParameters.BloomAmount;
				Parameters->shape = CRTShaderParameters.Shape;
				Parameters->DIFFUSION = CRTShaderParameters.Diffusion;

				FPixelShaderUtils::AddFullscreenPass(GraphBuilder, GlobalShaderMap, RDG_EVENT_NAME("Scanline"),
				                                     ScanlineShader, Parameters, Viewport,
				                                     TStaticBlendState<
					                                     CW_RGBA, BO_Add, BF_SourceAlpha,
					                                     BF_InverseSourceAlpha>::GetRHI());
			}


			GraphBuilder.Execute();
		}
	);
}

void UMrgCustomPassRenderer::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Render();
}

void UMrgCustomPassRenderer::Linearize_RenderThread(FRHICommandListImmediate& RHICmdList,
                                                    const ERHIFeatureLevel::Type FeatureLevel)
{
	check(IsInRenderingThread());
}
