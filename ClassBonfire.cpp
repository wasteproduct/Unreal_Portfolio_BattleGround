// Fill out your copyright notice in the Description page of Project Settings.

#include "ClassBonfire.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Materials/Material.h"
#include "PlayerControllerBattleRoyal.h"
#include "CharacterPlayerBase.h"
#include "UnrealNetwork.h"

// Sets default values
AClassBonfire::AClassBonfire()
	:woodComponent(nullptr), fire(nullptr), light(nullptr), sound(nullptr), collider(nullptr), explosion(nullptr), exploded(false),
	burnedMaterial(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//

	woodComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WoodComponent"));
	woodComponent->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh>wood(TEXT("/Game/Geometry/Meshes/1M_Cube"));

	ConstructorHelpers::FObjectFinder<UMaterial>woodMaterial(TEXT("/Game/StarterContent/Materials/M_Wood_Oak"));

	if (wood.Succeeded())
	{
		woodComponent->SetStaticMesh(wood.Object);
		woodComponent->SetWorldScale3D(FVector(0.64f));
		woodComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -32.0f));
	}

	if (woodMaterial.Succeeded())
	{
		woodComponent->SetMaterial(0, woodMaterial.Object);
	}

	//

	fire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire"));
	fire->SetupAttachment(woodComponent);

	ConstructorHelpers::FObjectFinder<UParticleSystem>fireParticle(TEXT("/Game/StarterContent/Particles/P_Fire"));

	if (fireParticle.Succeeded())
	{
		fire->SetTemplate(fireParticle.Object);
		fire->SetRelativeLocation(FVector(0.0f, 0.0f, 32.0f));
	}

	//

	light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	light->SetupAttachment(fire);

	FLinearColor lightColor;
	lightColor.R = 1.0f;
	lightColor.G = 0.8f;
	lightColor.B = 0.6f;
	lightColor.A = 1.0f;

	light->SetLightColor(lightColor);

	//

	sound = CreateDefaultSubobject<UAudioComponent>(TEXT("FireSound"));
	sound->SetupAttachment(fire);

	ConstructorHelpers::FObjectFinder<USoundCue>fireSound(TEXT("/Game/StarterContent/Audio/Fire01_Cue"));

	if (fireSound.Succeeded())
	{
		sound->SetSound(fireSound.Object);
	}

	//

	collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	collider->SetupAttachment(woodComponent);
	collider->SetWorldScale3D(FVector(1.0f));
	collider->SetSphereRadius(64.0f);

	collider->OnComponentBeginOverlap.AddDynamic(this, &AClassBonfire::Burn);

	//

	//woodComponent->SetSimulatePhysics(true);
	this->SetReplicates(true);
	NetCullDistanceSquared = 1000000.0f;

	//

	ConstructorHelpers::FObjectFinder<UParticleSystem>explosionObject(TEXT("/Game/StarterContent/Particles/P_Explosion"));

	if (explosionObject.Succeeded())
	{
		explosion = explosionObject.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterial>burnedMaterialObject(TEXT("/Game/Characters/Material_BurnedWood"));

	if (burnedMaterialObject.Succeeded())
	{
		burnedMaterial = burnedMaterialObject.Object;
	}
}

// Called when the game starts or when spawned
void AClassBonfire::BeginPlay()
{
	Super::BeginPlay();

	/*if (HasAuthority() == true)
	{
		UKismetSystemLibrary::PrintString(this, FString("I'm the damn server."), true, true, FLinearColor::Green, 16.0f);
	}
	else
	{
		if (GetOwner() == UGameplayStatics::GetPlayerController(this, 0))
		{
			UKismetSystemLibrary::PrintString(this, FString("I'm your mama."), true, true, FLinearColor::Red, 16.0f);
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, FString("Fuck you."), true, true, FLinearColor::Blue, 16.0f);
		}
	}*/
}

// Called every frame
void AClassBonfire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AClassBonfire::Burn(UPrimitiveComponent * overlappedComponent, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResult)
{
	if (HasAuthority() == true)
	{
		ACharacterPlayerBase *overlappedPlayer = nullptr;
		overlappedPlayer = Cast<ACharacterPlayerBase>(otherActor);

		if (overlappedPlayer)
		{
			//BoxKicked();
			if (exploded == false)
			{
				BlowBox();
				exploded = true;
			}
		}
	}
	else
	{
		
	}
}

void AClassBonfire::ChangeMaterial(void)
{
	if (exploded == false)return;

	if (burnedMaterial)
	{
		woodComponent->SetMaterial(0, burnedMaterial);
	}
	sound->SetSound(nullptr);
	fire->SetTemplate(nullptr);
	light->SetLightColor(FLinearColor::Transparent);
}

void AClassBonfire::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AClassBonfire, exploded);
}

bool AClassBonfire::BlowBox_Validate()
{
	return true;
}

void AClassBonfire::BlowBox_Implementation()
{
	if (explosion)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, explosion, GetActorLocation());
	}
}

bool AClassBonfire::BoxKicked_Validate()
{
	return true;
}

void AClassBonfire::BoxKicked_Implementation()
{
	UKismetSystemLibrary::PrintString(this, FString("I've got my shit kicked."), true, true, FLinearColor::Red, 4.0f);
}

