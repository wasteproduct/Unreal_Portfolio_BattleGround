// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPortal.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class PRACTICEACTOR_API AActorPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void Warp(UPrimitiveComponent *overlappedComponent, AActor *otherActor, UPrimitiveComponent *otherComponent,
			int32 otherBodyIndex, bool fromSweep, const FHitResult&sweepResult);

private:
	UStaticMeshComponent * panel;
	UBoxComponent *triggerBox;
};
