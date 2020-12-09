


#include "GSBlueprintLibrary.h"

#include "Kismet/KismetMathLibrary.h"


#include "SG/Character/ECharacter.h"

void UGSBlueprintLibrary::ConvertWorldToLocal(const FGSComponentAndTransform& WorldSpaceComponent, FGSComponentAndTransform& LocalSpaceComponent)
{
	LocalSpaceComponent.Component = WorldSpaceComponent.Component;
	LocalSpaceComponent.Transform = WorldSpaceComponent.Transform * (UKismetMathLibrary::InvertTransform(WorldSpaceComponent.Component->GetComponentToWorld()));
}

void UGSBlueprintLibrary::ConvertLocalToWorld(const FGSComponentAndTransform& LocalSpaceComponent, FGSComponentAndTransform& WorldSpaceComponent)
{
	WorldSpaceComponent.Component = LocalSpaceComponent.Component;
	WorldSpaceComponent.Transform = LocalSpaceComponent.Transform * LocalSpaceComponent.Component->GetComponentToWorld();
}
