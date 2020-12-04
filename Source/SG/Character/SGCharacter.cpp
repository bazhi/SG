


#include "SGCharacter.h"

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

