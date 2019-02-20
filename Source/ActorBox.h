// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorBox.generated.h"

class UBoxComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class PRACTICEACTOR_API AActorBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void ShowDebugMessageBeginOverlap(UPrimitiveComponent *overlappedComponent, AActor *otherActor,
			UPrimitiveComponent *otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult&sweepResult);

	UFUNCTION()
		void ShowDebugMessageEndOverlap(UPrimitiveComponent *overlappedComponent, AActor *otherActor,
			UPrimitiveComponent *otherComponent, int32 otherBodyIndex);

private:
	UBoxComponent * triggerBox;
	USphereComponent *triggerSphere;
	UStaticMeshComponent *boxBody;
};
