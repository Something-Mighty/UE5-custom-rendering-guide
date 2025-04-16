#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MrgPostEffectSubsystem.generated.h"

class FMrgSceneViewExtension;

UCLASS()
class UMrgPostEffectSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	TSharedPtr<FMrgSceneViewExtension> SceneViewExtension;
};
