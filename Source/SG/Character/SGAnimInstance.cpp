#include "SGAnimInstance.h"
#include "SGCharacter.h"

void USGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void USGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void USGAnimInstance::UpdateCharacterInfo()
{
	if(IsValid(Character))
	{
	  
	}
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
