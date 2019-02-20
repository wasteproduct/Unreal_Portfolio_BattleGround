// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PracticeActorCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// APracticeActorCharacter

//Custom
#include "Engine.h"
#include "ClassBonfire.h"
#include "CustomGameInstance.h"

APracticeActorCharacter::APracticeActorCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void APracticeActorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APracticeActorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APracticeActorCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APracticeActorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APracticeActorCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APracticeActorCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APracticeActorCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APracticeActorCharacter::OnResetVR);

	//Custom
	PlayerInputComponent->BindAction("SpawnBonfire", IE_Pressed, this, &APracticeActorCharacter::DropBonfire);
	PlayerInputComponent->BindAction("ViewDebugMessage", IE_Pressed, this, &APracticeActorCharacter::ShowDebugMessage);
	PlayerInputComponent->BindAction("RetrieveBonfire", IE_Pressed, this, &APracticeActorCharacter::RetrieveBonfire);
	PlayerInputComponent->BindAction("Possession", IE_Pressed, this, &APracticeActorCharacter::Possession);
}

void APracticeActorCharacter::Burn(void)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, "It's damn hot!");
	}

	UCustomGameInstance *gameInstance = Cast<UCustomGameInstance>(GetGameInstance());

	if (gameInstance)
	{
		gameInstance->SetBurned(true);
	}
}

void APracticeActorCharacter::DropBonfire(void)
{
	FVector direction = this->GetTransform().GetRotation().GetForwardVector();
	FVector location;
	location = this->GetTransform().GetLocation() + direction * 200.0f;
	location.Z = 130.0f;

	FTransform initialTransform = this->GetTransform();
	initialTransform.SetLocation(location);

	GetWorld()->SpawnActor<AClassBonfire>(AClassBonfire::StaticClass(), initialTransform);
}

void APracticeActorCharacter::ShowDebugMessage(void)
{
	UCustomGameInstance *gameInstance = Cast<UCustomGameInstance>(GetGameInstance());

	if (gameInstance)
	{
		bool burned = gameInstance->GetBurned();
		FString debugMessage;

		if (burned == true)debugMessage = "TRUE";
		else debugMessage = "FALSE";

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, debugMessage);
		}

		gameInstance->SetBurned(false);
	}
}

void APracticeActorCharacter::RetrieveBonfire(void)
{
	TArray<TEnumAsByte<EObjectTypeQuery>>objectType;
	objectType.Emplace(ECollisionChannel::ECC_WorldDynamic);

	TArray<AActor *>actorsToIgnore;
	TArray<AActor *>outActors;

	bool result = false;

	result = UKismetSystemLibrary::SphereOverlapActors(this, this->GetTransform().GetLocation(), 200.0f,
		objectType, AClassBonfire::StaticClass(), actorsToIgnore, outActors);

	if (result == true)
	{
		for (int32 i = outActors.Num() - 1; i >= 0; --i)
		{
			AClassBonfire *outActor = Cast<AClassBonfire>(outActors[i]);

			if (outActor)
			{
				outActor->Destroy();
			}
		}
	}
}

void APracticeActorCharacter::Possession(void)
{
	TArray<TEnumAsByte<EObjectTypeQuery>>objectType;
	objectType.Emplace(ECollisionChannel::ECC_Pawn);

	TArray<AActor *>actorsToIgnore;
	actorsToIgnore.Emplace(this);

	TArray<AActor *>outActors;

	bool result = false;

	result = UKismetSystemLibrary::SphereOverlapActors(this, this->GetTransform().GetLocation(), 100.0f,
		objectType, APracticeActorCharacter::StaticClass(), actorsToIgnore, outActors);

	if (result == true)
	{
		for (int32 i = 0; i < outActors.Num(); ++i)
		{
			APracticeActorCharacter *outActor = Cast<APracticeActorCharacter>(outActors[i]);

			if (outActor)
			{
				AController *controller = GetController();

				if (controller)
				{
					controller->Possess(outActor);
					return;
				}
			}
		}
	}
}


void APracticeActorCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APracticeActorCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void APracticeActorCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void APracticeActorCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APracticeActorCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APracticeActorCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APracticeActorCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
