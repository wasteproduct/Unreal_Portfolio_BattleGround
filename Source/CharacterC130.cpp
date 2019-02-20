// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterC130.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameModeBattleRoyal.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
ACharacterC130::ACharacterC130()
	:bodyMesh(nullptr), noise(nullptr), started(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));

	bodyMesh->SetupAttachment(GetCapsuleComponent());

	ConstructorHelpers::FObjectFinder<UStaticMesh>c130Object(TEXT("/Game/Vehicles/C-130/Meshes/SM_C_130"));

	if (c130Object.Succeeded())
	{
		bodyMesh->SetStaticMesh(c130Object.Object);
		bodyMesh->AddWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
		bodyMesh->AddRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	}

	bodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//

	for (int32 i = 0; i < 4; ++i)
	{
		FString componentName;
		componentName = FString("Propeller");
		componentName += FString::FromInt(i + 1);

		propeller.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*componentName)));

		propeller[i]->SetupAttachment(bodyMesh);
		propeller[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh>propellerObject(TEXT("/Game/Vehicles/C-130/Meshes/SM_C_130_Propellar"));

	if (propellerObject.Succeeded())
	{
		for (int32 i = 0; i < propeller.Num(); ++i)
		{
			propeller[i]->SetStaticMesh(propellerObject.Object);
		}
	}

	propeller[0]->SetRelativeLocation(FVector(1022.0f, 238.0f, 403.0f));
	propeller[1]->SetRelativeLocation(FVector(-504.0f, 238.0f, 373.0f));
	propeller[2]->SetRelativeLocation(FVector(516.0f, 238.0f, 373.0f));
	propeller[3]->SetRelativeLocation(FVector(-1012.0f, 238.0f, 403.0f));

	//

	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	GetCharacterMovement()->DefaultWaterMovementMode = EMovementMode::MOVE_Flying;

	GetCharacterMovement()->MaxFlySpeed = 36000.0f;

	//

	noise = CreateDefaultSubobject<UAudioComponent>(TEXT("Noise"));

	noise->SetupAttachment(GetCapsuleComponent());

	ConstructorHelpers::FObjectFinder<USoundBase>noiseObject(TEXT("/Game/Vehicles/C-130/Sounds/C130_Lockheed_Hercules_NOISE__Cue"));

	if (noiseObject.Succeeded())
	{
		noise->SetSound(noiseObject.Object);
	}

	noise->bOverrideAttenuation = true;
	noise->AttenuationOverrides.AttenuationShapeExtents = FVector(25000.0f, 0.0f, 0.0f);

	//

	bAlwaysRelevant = true;
	Tags.Emplace(TEXT("C130"));
}

// Called when the game starts or when spawned
void ACharacterC130::BeginPlay()
{
	Super::BeginPlay();
	
	initialLocation = GetActorLocation();
}

// Called every frame
void ACharacterC130::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() == true)
	{
		if (started == true)
		{
			GetCharacterMovement()->AddInputVector(UKismetMathLibrary::GetForwardVector(GetActorRotation()));

			if (FVector(initialLocation - GetActorLocation()).Size() > 700000.0f)
			{
				AGameModeBattleRoyal *currentGameMode = nullptr;
				currentGameMode = Cast<AGameModeBattleRoyal>(UGameplayStatics::GetGameMode(this));
				
				if (currentGameMode)
				{
					currentGameMode->ClearC130();

					FTimerHandle inOutHandle;
					GetWorld()->GetTimerManager().SetTimer(inOutHandle, this, &ACharacterC130::StopC130, 5.0f);
				}
			}
		}
	}
	else
	{
		for (int32 i = 0; i < propeller.Num(); ++i)
		{
			propeller[i]->AddLocalRotation(FRotator(DeltaTime*-720.0f, 0.0f, 0.0f));
		}
	}
}

// Called to bind functionality to input
void ACharacterC130::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterC130::SetStarted(bool flag)
{
	started = flag;
}

FVector ACharacterC130::GetMeshLocation(void)
{
	return bodyMesh->GetComponentLocation();
}

FVector ACharacterC130::GetMeshForwardVector(void)
{
	return bodyMesh->GetForwardVector();
}

void ACharacterC130::StopC130()
{
	started = false;

	SetActorTickEnabled(false);
}

