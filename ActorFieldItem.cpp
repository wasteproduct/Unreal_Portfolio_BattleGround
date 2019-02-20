// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorFieldItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ItemClass.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UnrealNetwork.h"

// Sets default values
AActorFieldItem::AActorFieldItem()
	:sphere(nullptr), mesh(nullptr), dataTable(nullptr), itemClassID(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));

	sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	RootComponent = sphere;

	//

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	mesh->SetupAttachment(RootComponent);

	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//

	ConstructorHelpers::FObjectFinder<UDataTable>tableObject(TEXT("/Game/Item/ItemClassTable"));

	if (tableObject.Succeeded())
	{
		dataTable = tableObject.Object;
	}

	//
}

// Called when the game starts or when spawned
void AActorFieldItem::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Server_SetupItemType();
	}

	//Server_SetupItemType();

	/*if (itemClassID != 0)
	{
		FString classID;
		classID = FString::FromInt((int32)itemClassID);

		FItemClass *tableRow;
		tableRow = dataTable->FindRow<FItemClass>(FName(*classID), FString("Failed"));

		if (tableRow)
		{
			mesh->SetStaticMesh(tableRow->staticMesh);

			if (tableRow->itemEquipmentType == EItemEquipmentType::Equipment_Hands)
			{
				mesh->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
				mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 4.0f));
			}
		}
	}*/

	/*FVector origin, boxExtent;
	float sphereRadius;

	UKismetSystemLibrary::GetComponentBounds(mesh, origin, boxExtent, sphereRadius);

	sphere->SetSphereRadius(sphereRadius);*/
}

// Called every frame
void AActorFieldItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AActorFieldItem::Server_SetupItemType_Validate()
{
	return true;
}

void AActorFieldItem::Server_SetupItemType_Implementation()
{
	itemClassID = 101 + FMath::RandHelper(5);
}

void AActorFieldItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AActorFieldItem, itemClassID);
}

void AActorFieldItem::OnRep_LoadItemMesh()
{
	if (itemClassID != 0)
	{
		FItemClass *tableRow = dataTable->FindRow<FItemClass>(*FString::FromInt((int32)itemClassID), FString("Failed"));

		if (tableRow)
		{
			mesh->SetStaticMesh(tableRow->staticMesh);

			if (tableRow->itemEquipmentType == EItemEquipmentType::Equipment_Hands)
			{
				mesh->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
				mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 4.0f));
			}
		}
	}

	FVector origin, boxExtent;
	float sphereRadius;

	UKismetSystemLibrary::GetComponentBounds(mesh, origin, boxExtent, sphereRadius);

	sphere->SetSphereRadius(sphereRadius);
}

