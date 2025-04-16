#include "MrgDynamicMeshComponent.h"

#include "DynamicMesh/MrgSceneProxy.h"

UMrgDynamicMeshComponent::UMrgDynamicMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FPrimitiveSceneProxy* UMrgDynamicMeshComponent::CreateSceneProxy()
{
	ensure(SceneProxy == nullptr);
	return new FMrgSceneProxy(this);
}

void UMrgDynamicMeshComponent::GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials,
                                                bool bGetDebugMaterials) const
{
	OutMaterials.Add(Material);
}

FBoxSphereBounds UMrgDynamicMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds(FSphere(FVector::ZeroVector, Radius)).TransformBy(LocalToWorld);
}

#if WITH_EDITOR
void UMrgDynamicMeshComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);


	if (PropertyChangedEvent.Property &&
		(PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UMrgDynamicMeshComponent, Radius) ||
			PropertyChangedEvent.Property->GetFName() ==
			GET_MEMBER_NAME_CHECKED(UMrgDynamicMeshComponent, VertexCount)))
	{
		MarkRenderStateDirty();
	}
}
#endif
