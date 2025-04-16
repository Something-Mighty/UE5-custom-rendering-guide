#pragma once
#include "SceneViewExtension.h"

class FMrgSceneViewExtension final : public FSceneViewExtensionBase
{
public:
	FMrgSceneViewExtension(const FAutoRegister& AutoRegister);

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override
	{
	};

	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override
	{
	};

	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override
	{
	};

	virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View,
	                                             const FPostProcessingInputs& Inputs) override;

private:
	static bool ShouldRender(const FSceneView& View);
};
