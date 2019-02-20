// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPortal.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PracticeActorCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AActorPortal::AActorPortal()
	:panel(nullptr), triggerBox(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	panel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>panelMesh(TEXT("/Engine/BasicShapes/Cube"));
	
	if (panelMesh.Succeeded())
	{
		panel->SetStaticMesh(panelMesh.Object);
		panel->SetWorldScale3D(FVector(1.0f, 1.0f, 0.08f));
		panel->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	}

	//

	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));

	if (triggerBox != NULL)
	{
		triggerBox->SetupAttachment(panel);
		triggerBox->SetRelativeLocation(FVector(0.0f, 0.0f, 528.0f));
		triggerBox->SetBoxExtent(FVector(50.0f, 50.0f, 500.0f));

		triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AActorPortal::Warp);
	}
}

// Called when the game starts or when spawned
void AActorPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorPortal::Warp(UPrimitiveComponent * overlappedComponent, AActor * otherActor, UPrimitiveComponent * otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult & sweepResult)
{
	if (otherActor == nullptr)return;

	APracticeActorCharacter *collidedCharacter = Cast<APracticeActorCharacter>(otherActor);

	if (collidedCharacter != nullptr)
	{
		FName levelName;
		FString currentLevelName = UGameplayStatics::GetCurrentLevelName(this);
		int32 result = 1;

		result = currentLevelName.Compare("BaseCamp");
		if (result == 0)levelName = "ThirdPersonExampleMap";

		result = currentLevelName.Compare("ThirdPersonExampleMap");
		if (result == 0)levelName = "BaseCamp";

		UGameplayStatics::OpenLevel(this, levelName);
	}
}

