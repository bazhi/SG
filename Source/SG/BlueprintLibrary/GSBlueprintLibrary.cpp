


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

FTransform UGSBlueprintLibrary::TransformSubtraction(const FTransform& A, const FTransform B)
{
	FTransform Result;
	Result.SetLocation(A.GetLocation() - B.GetLocation());
	Result.SetRotation((A.Rotator() - B.Rotator()).Quaternion());
	Result.SetScale3D(A.GetScale3D() - B.GetScale3D());
	return  Result;
}

FTransform UGSBlueprintLibrary::TransformAddition(const FTransform& A, const FTransform B)
{
	FTransform Result;
	Result.SetLocation(A.GetLocation() + B.GetLocation());
	Result.SetRotation((A.Rotator() + B.Rotator()).Quaternion());
	Result.SetScale3D(A.GetScale3D() + B.GetScale3D());
	return  Result;
}
