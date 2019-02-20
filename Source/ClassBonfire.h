// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClassBonfire.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;
class UPointLightComponent;
class UAudioComponent;
class USphereComponent;
class UParticleSystem;
class UMaterial;

UCLASS()
class PRACTICEACTOR_API AClassBonfire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClassBonfire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
		void Burn(UPrimitiveComponent *overlappedComponent, AActor *otherActor, UPrimitiveComponent *otherComp,
			int32 otherBodyIndex, bool fromSweep, const FHitResult&sweepResult);

	UFUNCTION(Client, Reliable, WithValidation)
		void BoxKicked();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void BlowBox();

	UFUNCTION()
		void ChangeMaterial();

protected:
	UPROPERTY(VisibleAnywhere)
		USphereComponent *collider;

	UPROPERTY(ReplicatedUsing = ChangeMaterial)
		bool exploded;

private:
	UStaticMeshComponent * woodComponent;
	UParticleSystemComponent *fire;
	UPointLightComponent *light;
	UAudioComponent *sound;

	UParticleSystem *explosion;
	UMaterial *burnedMaterial;
};
