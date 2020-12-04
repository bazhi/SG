#include "SGCharacter.h"

#include "Components/CapsuleComponent.h"


#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASGCharacter::ASGCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASGCharacter::BeginPlay()
{
    Super::BeginPlay();
    OnBeginPlay();
}

void ASGCharacter::OnBeginPlay()
{
}

// Called every frame
void ASGCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASGCharacter::GetControlForwardAndRightVector(FVector& Forward, FVector& Right)
{
    FRotator Rotator = GetControlRotation();
    Forward = UKismetMathLibrary::GetForwardVector(FRotator(0, 0, Rotator.Yaw));
    Right = UKismetMathLibrary::GetRightVector(FRotator(0, 0, Rotator.Yaw));
}

void ASGCharacter::GetCapsuleBaseLocation(float ZOffset, FVector& ReturnValue)
{
    auto CapsuleComponentTemp = GetCapsuleComponent();
    if(CapsuleComponentTemp)
    {
        FVector UpVector = CapsuleComponentTemp->GetUpVector();
        float HalfHeight = CapsuleComponentTemp->GetScaledCapsuleHalfHeight();
        FVector WorldLocation = CapsuleComponentTemp->GetComponentLocation();
        ReturnValue = WorldLocation - UpVector * (ZOffset + HalfHeight);
    }
}
