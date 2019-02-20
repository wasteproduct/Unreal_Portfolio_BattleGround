// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterC130.generated.h"

class UStaticMeshComponent;
class UAudioComponent;
class USphereComponent;

UCLASS()
class PRACTICEACTOR_API ACharacterC130 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterC130();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetStarted(bool flag);

	FVector GetMeshLocation(void);
	FVector GetMeshForwardVector(void);

protected:
	UFUNCTION()
		void StopC130();

private:
	UStaticMeshComponent * bodyMesh;
	TArray<UStaticMeshComponent *>propeller;

	UAudioComponent *noise;

	bool started;

	FVector initialLocation;
};
