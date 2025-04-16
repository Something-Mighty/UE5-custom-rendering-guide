#include "MrgPostEffectSubsystem.h"

#include "MrgSceneViewExtension.h"
#include "SceneViewExtension.h"

void UMrgPostEffectSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SceneViewExtension = FSceneViewExtensions::NewExtension<FMrgSceneViewExtension>();
}

void UMrgPostEffectSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
