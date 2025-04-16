#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "MrgDynamicMeshComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MIGHTYRENDERINGGUIDEGFX_API UMrgDynamicMeshComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

	friend class FMrgSceneProxy;

public:
	UMrgDynamicMeshComponent();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> Material;

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual void GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials,
	                              bool bGetDebugMaterials = false) const override;

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	UPROPERTY(EditAnywhere)
	float Radius = 10.0f;

	UPROPERTY(EditAnywhere)
	uint16 VertexCount = 512;

	UPROPERTY(EditAnywhere)
	float WaveAmplitude = 0.5f;

	UPROPERTY(EditAnywhere)
	float WaveFrequency = 32.0f;
};
