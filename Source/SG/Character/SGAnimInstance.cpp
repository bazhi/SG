#include "SGAnimInstance.h"
#include "SGCharacter.h"

void USGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ASGCharacter>(TryGetPawnOwner());
}

void USGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	DeltaTimeX = DeltaSeconds;
	if(DeltaTimeX > 0.0f && IsValid(Character))
	{
		UpdateCharacterInfo();
		UpdateAimingValues();
		UpdateLayerValues();
		UpdateFootIK();

        switch (MovementState)
        {
            case EMovementState::None:
                break;
            case EMovementState::Grounded:
				bool ShouldMove = ShouldMoveCheck();
			    if(ShouldMove)
			    {
			        if(!bGroundMoved)
			        {
						ElapsedDelayTime = 0;
						RotateL = false;
						RotateR = false;
						bGroundMoved = true;
			        }
					UpdateMovementValues();
					UpdateRotationValues();
			    }else
			    {
					bGroundMoved = false;

			    }
                break;
            case EMovementState::InAir:
				UpdateInAirValues();
                break;
            case EMovementState::Mantling:
                break;
            case EMovementState::Ragdoll:
				UpdateRagdollValues();
                break;
            default: ;
        }
	}
}

void USGAnimInstance::UpdateCharacterInfo()
{
	if(IsValid(Character))
	{
	  
	}
}

void USGAnimInstance::UpdateAimingValues()
{
}

void USGAnimInstance::UpdateLayerValues()
{
}

void USGAnimInstance::UpdateMovementValues()
{
}

void USGAnimInstance::UpdateRotationValues()
{
}

void USGAnimInstance::UpdateInAirValues()
{
}

void USGAnimInstance::UpdateRagdollValues()
{
}

void USGAnimInstance::UpdateFootIK()
{
}

bool USGAnimInstance::ShouldMoveCheck() const
{
	return IsMoving && HasMovementInput || Speed > 150.0f;
}

float USGAnimInstance::GetAnimCurveCompact(const FName& CurveName)
{
	return GetCurveValue(CurveName);
}

float USGAnimInstance::GetAnimCurveClamped(const FName& CurveName, float Bias, float ClampMin, float ClampMax)
{
	float Value = GetCurveValue(CurveName);
	return FMath::Clamp(Value + Bias, ClampMin, ClampMax);
}
