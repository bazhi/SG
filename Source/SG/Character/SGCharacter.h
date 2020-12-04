

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ECharacter.h"
#include "SGCharacter.generated.h"

UCLASS()
class SG_API ASGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	void OnBeginPlay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
