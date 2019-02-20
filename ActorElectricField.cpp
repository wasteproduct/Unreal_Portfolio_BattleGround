// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorElectricField.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameStateBattleRoyal.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AActorElectricField::AActorElectricField()
	:electricCylinder(nullptr), currentGameState(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//

	electricCylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	
	electricCylinder->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh>electricCylinderObject(TEXT("/Game/EletricField/SM_EletricCylinder"));

	if (electricCylinderObject.Succeeded())
	{
		electricCylinder->SetStaticMesh(electricCylinderObject.Object);
	}
}

// Called when the game starts or when spawned
void AActorElectricField::BeginPlay()
{
	Super::BeginPlay();

	currentGameState = Cast<AGameStateBattleRoyal>(UGameplayStatics::GetGameState(this));
}

// Called every frame
void AActorElectricField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(currentGameState) == true)
	{
		electricCylinder->SetVisibility(currentGameState->GetShowingCurrentCircle(), true);
		SetActorLocation(currentGameState->GetCurrentCircleLocation(), false, (FHitResult *)nullptr, ETeleportType::TeleportPhysics);

		float diameter;
		diameter = currentGameState->GetCurrentCircleRadius() / 100.0f*2.0f;

		SetActorScale3D(FVector(diameter, diameter, 1000.0f));
	}
}

