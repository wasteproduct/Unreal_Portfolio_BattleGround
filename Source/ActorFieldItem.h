// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorFieldItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UDataTable;

UCLASS()
class PRACTICEACTOR_API AActorFieldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorFieldItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps)const override;

	int GetItemClassID(void) { return itemClassID; }
	USphereComponent *GetSphere(void) { return sphere; }

public:
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetupItemType();

	UFUNCTION()
		virtual void OnRep_LoadItemMesh();

public:
	UPROPERTY(VisibleAnywhere)
		USphereComponent *sphere;

	UPROPERTY(ReplicatedUsing = OnRep_LoadItemMesh)
		int itemClassID;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent *mesh;

	UPROPERTY()
		UDataTable *dataTable;
};
