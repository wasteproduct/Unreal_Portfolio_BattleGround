// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorBox.h"
//#include "Components/BoxComponent.h"
//#include "Components/SphereComponent.h"
//#include "Components/StaticMeshComponent.h"
//#include "UObject/ConstructorHelpers.h"
#include "Engine.h"

// Sets default values
AActorBox::AActorBox()
	:triggerBox(nullptr), triggerSphere(nullptr), boxBody(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>boxMesh(TEXT("/Engine/BasicShapes/Cube"));

	if (boxMesh.Succeeded())
	{
		boxBody->SetStaticMesh(boxMesh.Object);
		boxBody->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	}

	//

	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));

	if (triggerBox != NULL)
	{
		triggerBox->SetupAttachment(boxBody);
		triggerBox->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
		triggerBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

		triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AActorBox::ShowDebugMessageBeginOverlap);
		triggerBox->OnComponentEndOverlap.AddDynamic(this, &AActorBox::ShowDebugMessageEndOverlap);
	}

	//

	triggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));

	if (triggerSphere != NULL)
	{
		triggerSphere->SetupAttachment(boxBody);
		triggerSphere->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
		triggerSphere->SetSphereRadius(150.0f);
	}
}

// Called when the game starts or when spawned
void AActorBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorBox::ShowDebugMessageBeginOverlap(UPrimitiveComponent * overlappedComponent, AActor * otherActor, UPrimitiveComponent * otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue, "Blue is a good velociraptor");
	}
}

void AActorBox::ShowDebugMessageEndOverlap(UPrimitiveComponent * overlappedComponent, AActor * otherActor, UPrimitiveComponent * otherComponent, int32 otherBodyIndex)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, "I know, right?");
	}
}

