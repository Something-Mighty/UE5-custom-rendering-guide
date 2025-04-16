#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MrgCustomPassRenderer.generated.h"

USTRUCT(BlueprintType)
struct MIGHTYRENDERINGGUIDEGFX_API FMrgCRTShaderParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HardScan = -8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HardPix = -3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarpX = 0.031f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarpY = 0.041f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaskDark = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaskLight = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleInLinearGamma = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShadowMask = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrightBoost = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HardBloomScan = -2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HardBloomPix = -1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BloomAmount = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Shape = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Diffusion = 0.0f;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MIGHTYRENDERINGGUIDEGFX_API UMrgCustomPassRenderer : public UActorComponent
{
	GENERATED_BODY()

public:
	UMrgCustomPassRenderer();

	UFUNCTION()
	void Render();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTextureRenderTarget2D> Source;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTextureRenderTarget2D> Target;

	UPROPERTY(EditAnywhere)
	FMrgCRTShaderParameters ShaderParameters;

private:
	static void Linearize_RenderThread(FRHICommandListImmediate& RHICmdList, ERHIFeatureLevel::Type FeatureLevel);
};
