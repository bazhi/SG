#include "SGPlayerCameraManager.h"

#include "SGCameraAnimInstance.h"
#include "SG/Core/SGName.h"

ASGPlayerCameraManager::ASGPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	CameraMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>("CameraMesh");
	if (CameraMesh)
	{
		CameraMesh->AlwaysLoadOnClient = true;
		CameraMesh->AlwaysLoadOnServer = true;
		CameraMesh->bOwnerNoSee = false;
		CameraMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		CameraMesh->bCastDynamicShadow = false;
		CameraMesh->bAffectDynamicIndirectLighting = false;
		CameraMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		CameraMesh->SetupAttachment(RootComponent);
		static FName MeshCollisionProfileName(TEXT("CameraMesh"));
		CameraMesh->SetCollisionProfileName(MeshCollisionProfileName);
		CameraMesh->SetGenerateOverlapEvents(false);
		CameraMesh->SetCanEverAffectNavigation(false);
		CameraMesh->SetHiddenInGame(true);
		CameraMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASGPlayerCameraManager::OnPossess(APawn* InPawn)
{
    ControllerdPawn = InPawn;
	ICamera = InPawn;
	if(auto AnimInstance = Cast<USGCameraAnimInstance>(CameraMesh->GetAnimInstance()))
	{
		AnimInstance->UpdateController(GetOwningPlayerController(), InPawn);
	}
}

void ASGPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTargetInternal(OutVT, DeltaTime);
	if (auto Target = GetViewTarget())
	{
		if (Target->ActorHasTag(SGName::GameTag::SGCharacter))
		{
			UpdateCameraBehavior(OutVT.POV.Location, OutVT.POV.Rotation, OutVT.POV.FOV);
		}
	}
}

void ASGPlayerCameraManager::UpdateCameraBehavior(FVector& OutLocation, FRotator& OutRotation, float& OutFOV)
{
	if(ICamera)
	{
        FTransform PivotTarget = ICamera->Get3PPivotTarget();
		FVector FPTarget = ICamera->GetFpCameraTarget();
		float TpFov;
		float FpFov;
		bool bRightShoulder;
		ICamera->GetCameraParameters(TpFov, FpFov, bRightShoulder);


	}
}
