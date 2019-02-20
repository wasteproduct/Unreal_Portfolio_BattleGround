// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnFlyingViewer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "UserWidgetInteraction.h"
#include "Components/SphereComponent.h"
#include "PlayerControllerBattleRoyal.h"

// Sets default values
APawnFlyingViewer::APawnFlyingViewer()
	:springArm(nullptr), camera(nullptr), widget(nullptr), widgetInteraction(nullptr)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bOnlyRelevantToOwner = true;
	
	bReplicateMovement = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	//
	
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	
	springArm->SetupAttachment(RootComponent);
	springArm->TargetArmLength = 3000.0f;
	springArm->bUsePawnControlRotation = true;
	springArm->bDoCollisionTest = false;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	camera->SetupAttachment(springArm);

	//

	widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

	widget->SetupAttachment(RootComponent);
	widget->SetWidgetSpace(EWidgetSpace::Screen);
	widget->SetRelativeLocation(FVector(0.0f, 0.0f, 640.0f));
	
	ConstructorHelpers::FClassFinder<UUserWidgetInteraction>widgetClass(TEXT("/Game/Characters/HUD/Blueprint_Widget_Interaction"));

	if (widgetClass.Succeeded())
	{
		widget->SetWidgetClass(widgetClass.Class);

		widget->SetDrawAtDesiredSize(true);
	}
}

// Called when the game starts or when spawned
void APawnFlyingViewer::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() == true)
	{

	}
	else
	{
		if (widget)
		{
			widgetInteraction = Cast<UUserWidgetInteraction>(widget->GetUserWidgetObject());
		}

		if (widgetInteraction)
		{
			widgetInteraction->SetInteractionText(TEXT("Jump"));
		}
	}
}

// Called every frame
void APawnFlyingViewer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnFlyingViewer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
		
		PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &APawnFlyingViewer::Interact);
	}
}

void APawnFlyingViewer::Interact()
{
	APlayerControllerBattleRoyal *controller = nullptr;
	controller = Cast<APlayerControllerBattleRoyal>(GetController());

	if (controller)
	{
		controller->Server_JumpOut();
	}
}

