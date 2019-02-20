// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterPlayerBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AnimationInstancePlayerBase.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "WidgetCrosshair.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "CameraShakeRecoil.h"
#include "ActorFieldItem.h"
#include "ItemClass.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "UserWidgetInteraction.h"
#include "PlayerControllerBattleRoyal.h"
#include "ClassBonfire.h"
#include "UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameStateBattleRoyal.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
ACharacterPlayerBase::ACharacterPlayerBase()
	:springArm(nullptr), camera(nullptr), moveForwardValue(0.0f), moveRightValue(0.0f), weaponBack(nullptr), weaponHeld(nullptr),
	weaponEquipped(false), aiming(false), mainWeapon(nullptr), animationFire(nullptr), rifleSound(nullptr), rifleFlare(nullptr),
	crosshair(nullptr), timelineCrosshair(nullptr), curveFieldOfView(nullptr), bulletEffect(nullptr), bloodEffect(nullptr),
	timelineShotRecovery(nullptr), curvePhysicsBlend(nullptr), hp(100.0f), maximumHP(100.0f), bodyUpper(nullptr), bodyLower(nullptr),
	feet(nullptr), hair(nullptr), itemDataTable(nullptr), widgetInteraction(nullptr), fieldItem(nullptr), carryingWeapon(false),
	previousMoveForwardValue(0.0f), previousMoveRightValue(0.0f), upperItemClassID(0), lowerItemClassID(0), headItemClassID(0),
	feetItemClassID(0), weaponItemClassID(0), dead(false), animationDie(nullptr), waistPitch(0.0f), previousWaistPitch(0.0f),
	falling(false), gliding(false), animationFalling(nullptr), animationGliding(nullptr), parachute(nullptr),
	timelineSpreadingParachute(nullptr), curveSpreadingParachute(nullptr), maximumDescendingPitch(0.0f), maximumDescendingRoll(0.0f),
	currentDescendingPitch(0.0f), currentDescendingRoll(0.0f), parachuteCloud(nullptr), windCue(nullptr), soundSpreadingParachute(nullptr)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>skeletalMeshObject(TEXT("/Game/Characters/Female/Parts/SK_Female_Body_Head"));

	if (skeletalMeshObject.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(skeletalMeshObject.Object);
		GetMesh()->SetRelativeRotation(FQuat(FVector::UpVector, -PI / 2.0f));
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		ConstructorHelpers::FObjectFinder<UClass>animationBlueprint(TEXT("/Game/Characters/AnimationBlueprint_PlayerBase.AnimationBlueprint_PlayerBase_C"));

		if (animationBlueprint.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(animationBlueprint.Object);
		}

		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		GetMesh()->ComponentTags.Emplace(FName("Body"));
	}

	//

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	if (springArm)
	{
		springArm->SetupAttachment(GetMesh());
		springArm->SetRelativeRotation(FQuat(FVector::UpVector, PI / 2.0f));
		springArm->SetWorldLocation(FVector(0.0f, 0.0f, 120.0f));
		springArm->TargetArmLength = 300.0f;
		springArm->ProbeSize = 100.0f;
		springArm->bUsePawnControlRotation = true;
		springArm->bInheritRoll = false;
	}

	//

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	if (camera)
	{
		camera->SetupAttachment(springArm);
		camera->FieldOfView = 70.0f;
		camera->SetRelativeLocation(FVector(-200.0f, 0.0f, 0.0f));
	}

	//

	weaponBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponBack"));

	if (weaponBack)
	{
		weaponBack->SetupAttachment(GetMesh(), "WeaponBack");

		weaponBack->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		weaponBack->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	}

	//

	weaponHeld = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponHeldAiming"));

	if (weaponHeld)
	{
		weaponHeld->SetupAttachment(GetMesh(), "WeaponHeld");

		weaponHeld->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		weaponHeld->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	}

	//

	ConstructorHelpers::FObjectFinder<UAnimMontage>fireMontage(TEXT("/Game/Characters/Female/Animations/Aiming_Fire_Montage"));

	if (fireMontage.Succeeded())
	{
		animationFire = fireMontage.Object;
	}

	//

	ConstructorHelpers::FObjectFinder<USoundBase>soundObject(TEXT("/Game/ArbitraryStudio/Weapons/rifle_sound"));

	if (soundObject.Succeeded())
	{
		rifleSound = soundObject.Object;
	}

	//

	ConstructorHelpers::FObjectFinder<UParticleSystem>particleObject(TEXT("/Game/WeaponEffects/P_AssaultRifle_MF"));

	if (particleObject.Succeeded())
	{
		rifleFlare = particleObject.Object;
	}

	//

	ConstructorHelpers::FClassFinder<UWidgetCrosshair>widgetClass(TEXT("/Game/Characters/HUD/Blueprint_Widget_Crosshair"));

	if (widgetClass.Succeeded())
	{
		subclassCrosshairWidget = widgetClass.Class;
	}

	//

	timelineCrosshair = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrosshairTimeline"));
	
	ConstructorHelpers::FObjectFinder<UCurveFloat>fieldOfViewCurveObject(TEXT("/Game/Timeline/CameraFieldOfView"));

	if (fieldOfViewCurveObject.Succeeded())
	{
		curveFieldOfView = fieldOfViewCurveObject.Object;
	}

	//

	ConstructorHelpers::FObjectFinder<UParticleSystem>bulletEffectObject(TEXT("/Game/WeaponEffects/P_AssaultRifle_IH"));

	if (bulletEffectObject.Succeeded())
	{
		bulletEffect = bulletEffectObject.Object;
	}

	//

	ConstructorHelpers::FObjectFinder<UParticleSystem>bloodEffectObject(TEXT("/Game/WeaponEffects/P_body_bullet_impact"));

	if (bloodEffectObject.Succeeded())
	{
		bloodEffect = bloodEffectObject.Object;
	}

	//

	timelineShotRecovery = CreateDefaultSubobject<UTimelineComponent>(TEXT("ShotRecoveryTimeline"));

	ConstructorHelpers::FObjectFinder<UCurveFloat>physicsBlendCurveObject(TEXT("/Game/Timeline/PhysicsBlend"));

	if (physicsBlendCurveObject.Succeeded())
	{
		curvePhysicsBlend = physicsBlendCurveObject.Object;
	}

	//
	
	bodyUpper = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UpperBody"));
	bodyLower = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LowerBody"));
	feet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
	hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));

	bodyUpper->SetupAttachment(GetMesh());
	bodyLower->SetupAttachment(GetMesh());
	feet->SetupAttachment(GetMesh());
	hair->SetupAttachment(GetMesh());
	
	bodyUpper->SetMasterPoseComponent(GetMesh());
	bodyLower->SetMasterPoseComponent(GetMesh());
	feet->SetMasterPoseComponent(GetMesh());
	hair->SetMasterPoseComponent(GetMesh());

	ConstructorHelpers::FObjectFinder<USkeletalMesh>upperBodyObject(TEXT("/Game/Characters/Female/Parts/SK_Female_Body_Upper"));
	if (upperBodyObject.Succeeded())
	{
		bodyUpper->SetSkeletalMesh(upperBodyObject.Object);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh>lowerBodyObject(TEXT("/Game/Characters/Female/Parts/SK_Female_Body_Lower"));
	if (lowerBodyObject.Succeeded())
	{
		bodyLower->SetSkeletalMesh(lowerBodyObject.Object);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh>hairObject(TEXT("/Game/Characters/Female/Parts/SK_Female_Cloth_Helmet_Hair"));
	if (hairObject.Succeeded())
	{
		hair->SetSkeletalMesh(hairObject.Object);
	}

	//

	ConstructorHelpers::FObjectFinder<UDataTable>tableObject(TEXT("/Game/Item/ItemClassTable"));
	
	if (tableObject.Succeeded())
	{
		itemDataTable = tableObject.Object;
	}

	//

	widgetInteraction = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetInteraction"));

	ConstructorHelpers::FClassFinder<UUserWidgetInteraction>interactionWidgetObject(TEXT("/Game/Characters/HUD/Blueprint_Widget_Interaction"));

	if (interactionWidgetObject.Succeeded())
	{
		widgetInteraction->SetWidgetClass(interactionWidgetObject.Class);

		widgetInteraction->SetupAttachment(GetMesh());
		widgetInteraction->SetRelativeLocation(FVector(-64.0f, 0.0f, 128.0f));

		widgetInteraction->SetWidgetSpace(EWidgetSpace::Screen);
		widgetInteraction->SetDrawSize(FVector2D(500.0f, 500.0f));
		widgetInteraction->SetPivot(FVector2D(0.0f, 0.5f));
		widgetInteraction->SetDrawAtDesiredSize(true);
	}

	//

	ConstructorHelpers::FObjectFinder<UAnimMontage>dieMontage(TEXT("/Game/Characters/Female/Animations/Pose_Dead_Montage"));

	if (dieMontage.Succeeded())
	{
		animationDie = dieMontage.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>fallingMontage(TEXT("/Game/Characters/Female/Animations/Flying_Montage"));

	if (fallingMontage.Succeeded())
	{
		animationFalling = fallingMontage.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>glidingMontage(TEXT("/Game/Characters/Female/Animations/Gliding_Montage"));

	if (glidingMontage.Succeeded())
	{
		animationGliding = glidingMontage.Object;
	}

	//

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	//NetCullDistanceSquared = 1000000.0f;

	//

	parachute = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Parachute"));

	parachute->SetupAttachment(GetMesh());
	parachute->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	parachute->SetVisibility(false);

	ConstructorHelpers::FObjectFinder<UStaticMesh>parachuteObject(TEXT("/Game/Vehicles/Parachute/Meshes/Parachute"));

	if (parachuteObject.Succeeded())
	{
		parachute->SetStaticMesh(parachuteObject.Object);

		parachute->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0));
		parachute->SetRelativeLocation(FVector(0.0f, 8.0f, 133.0f));
	}

	//

	timelineSpreadingParachute = CreateDefaultSubobject<UTimelineComponent>(TEXT("SpreadingParachuteTimeline"));

	ConstructorHelpers::FObjectFinder<UCurveFloat>spreadingParachuteCurveObject(TEXT("/Game/Timeline/SpreadParachute"));

	if (spreadingParachuteCurveObject.Succeeded())
	{
		curveSpreadingParachute = spreadingParachuteCurveObject.Object;
	}

	//

	parachuteCloud = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Cloud"));
	parachuteCloud->SetupAttachment(springArm);
	parachuteCloud->SetAutoActivate(false);
	
	ConstructorHelpers::FObjectFinder<UParticleSystem>cloudObject(TEXT("/Game/Vehicles/Parachute/Effects/P_Cloud"));

	if (cloudObject.Succeeded())
	{
		parachuteCloud->SetTemplate(cloudObject.Object);
		parachuteCloud->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		parachuteCloud->SetRelativeLocation(FVector(1700.0f, 0.0f, 0.0f));
	}

	windCue = CreateDefaultSubobject<UAudioComponent>(TEXT("WindCue"));
	windCue->SetupAttachment(parachuteCloud);
	
	ConstructorHelpers::FObjectFinder<USoundCue>windCueObject(TEXT("/Game/Vehicles/Parachute/Effects/wind_Cue"));

	if (windCueObject.Succeeded())
	{
		windCue->SetSound(windCueObject.Object);
		windCue->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		windCue->bOverrideAttenuation = true;
		windCue->SetAutoActivate(false);
	}

	ConstructorHelpers::FObjectFinder<USoundBase>soundSpreadingParachuteObject(TEXT("/Game/Vehicles/Parachute/Effects/parachute_opening"));

	if (soundSpreadingParachuteObject.Succeeded())
	{
		soundSpreadingParachute = soundSpreadingParachuteObject.Object;
	}
}

// Called when the game starts or when spawned
void ACharacterPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	hp = maximumHP;

	//서버
	if (HasAuthority() == true)
	{

	}
	//클라이언트
	else
	{
		//내 디바이스
		if (GetController() == UGameplayStatics::GetPlayerController(this, 0))
		{
			if (subclassCrosshairWidget)
			{
				APlayerControllerBattleRoyal *localPlayerController;
				localPlayerController = Cast<APlayerControllerBattleRoyal>(GetController());

				if (localPlayerController)
				{
					crosshair = CreateWidget<UWidgetCrosshair>(localPlayerController, subclassCrosshairWidget);
				}
			}
		}
		//남의 디바이스
		else
		{

		}
	}

	if (curveFieldOfView)
	{
		FOnTimelineFloat functionBridge;
		functionBridge.BindUFunction(this, FName("ChangeFieldOfView"));

		if (timelineCrosshair)
		{
			timelineCrosshair->AddInterpFloat(curveFieldOfView, functionBridge);
		}
	}

	if (curvePhysicsBlend)
	{
		FOnTimelineFloat onFunctionBridge;
		FOnTimelineEventStatic finishingFunctionBridge;
		onFunctionBridge.BindUFunction(this, FName("Shot"));
		finishingFunctionBridge.BindUFunction(this, FName("Recover"));

		if (timelineShotRecovery)
		{
			timelineShotRecovery->SetTimelineLength(1.0f);

			timelineShotRecovery->AddInterpFloat(curvePhysicsBlend, onFunctionBridge);
			timelineShotRecovery->SetTimelineFinishedFunc(finishingFunctionBridge);
		}
	}

	widgetInteraction->SetVisibility(false);

	if (curveSpreadingParachute)
	{
		FOnTimelineFloat functionBridge;
		functionBridge.BindUFunction(this, FName("CurveParachuteSpreadingTimeline"));

		if (timelineSpreadingParachute)
		{
			timelineSpreadingParachute->SetTimelineLength(0.5f);

			timelineSpreadingParachute->AddInterpFloat(curveSpreadingParachute, functionBridge);
		}
	}
}

// Called every frame
void ACharacterPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (timelineCrosshair)
	{
		timelineCrosshair->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}

	if (timelineShotRecovery)
	{
		timelineShotRecovery->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}

	//서버
	if (HasAuthority() == true)
	{

	}
	//클라이언트
	else
	{
		//내 디바이스
		if (GetController() == UGameplayStatics::GetPlayerController(this, 0))
		{
			if ((falling == true) || (gliding == true))
			{
				SequenceParachuteCode();

				SequenceDescendingRotationCode();
			}
			else
			{
				fieldItem = FindItemsSurrounding();

				if (fieldItem)
				{
					if (itemDataTable)
					{
						FString rowName;
						rowName = FString::FromInt((int32)fieldItem->GetItemClassID());

						FItemClass *tableRow;
						tableRow = itemDataTable->FindRow<FItemClass>(FName(*rowName), FString("HeyYouFailedLoser"));

						if (tableRow)
						{
							UUserWidgetInteraction *userWidgetInteraction;
							userWidgetInteraction = Cast<UUserWidgetInteraction>(widgetInteraction->GetUserWidgetObject());

							if (userWidgetInteraction)
							{
								userWidgetInteraction->SetInteractionText(tableRow->itemName);

								widgetInteraction->SetVisibility(true);
							}
						}
					}
				}
				else
				{
					widgetInteraction->SetVisibility(false);
				}
			}
		}
		//남의 디바이스
		else
		{

		}
	}

}

bool ACharacterPlayerBase::Server_Landing_Validate()
{
	return true;
}

void ACharacterPlayerBase::Server_Landing_Implementation()
{
	if ((falling == false) && (gliding == false))return;

	if (falling == true)falling = false;
	if (gliding == true)gliding = false;
}

void ACharacterPlayerBase::PlayAnimationFalling()
{
	if (falling == true)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(animationFalling);

		bUseControllerRotationYaw = false;

		GetCharacterMovement()->AirControl = 1.0f;
		GetCharacterMovement()->MaxWalkSpeed = 10000.0f;
		GetCharacterMovement()->GravityScale = 0.5f;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 10.0f, 0.0f);

		parachuteCloud->Activate(true);
		windCue->Activate(true);

		Server_MotionFalling();
	}
	else
	{
		StartWalking();
	}
}

void ACharacterPlayerBase::PlayAnimationGliding()
{
	if (gliding == true)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(animationGliding);

		parachute->SetVisibility(true, true);

		SpreadParachute();

		springArm->TargetArmLength = 700.0f;

		if (soundSpreadingParachute)
		{
			UGameplayStatics::PlaySound2D(this, soundSpreadingParachute);
		}

		GetCharacterMovement()->AirControl = 1.0f;
		GetCharacterMovement()->MaxWalkSpeed = 4000;
		GetCharacterMovement()->GravityScale = 0.1f;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 10.0f, 0.0f);
		GetCharacterMovement()->Velocity *= 0.5f;

		Server_MotionGliding();
	}
	else
	{
		StartWalking();
	}
}

void ACharacterPlayerBase::StartWalking(void)
{
	if ((falling == true) || (gliding == true))return;

	bUseControllerRotationYaw = true;

	parachute->SetVisibility(false, true);

	springArm->TargetArmLength = 300.0f;

	StopAnimMontage(animationFalling);
	StopAnimMontage(animationGliding);

	currentDescendingPitch = 0.0f;
	currentDescendingRoll = 0.0f;
	maximumDescendingPitch = 0.0f;
	maximumDescendingRoll = 0.0f;

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	windCue->Deactivate();

	GetCharacterMovement()->AirControl = 0.05f;
	GetCharacterMovement()->MaxWalkSpeed = 4000.0f;
	GetCharacterMovement()->GravityScale = 1.0f;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

	Server_StartWalking();
}

void ACharacterPlayerBase::SpreadParachute(void)
{
	if (timelineSpreadingParachute)
	{
		timelineSpreadingParachute->PlayFromStart();
	}
}

bool ACharacterPlayerBase::Server_StartWalking_Validate()
{
	return true;
}

void ACharacterPlayerBase::Server_StartWalking_Implementation()
{
	bUseControllerRotationYaw = true;

	parachute->SetVisibility(false, true);

	springArm->TargetArmLength = 300.0f;

	StopAnimMontage(animationFalling);
	StopAnimMontage(animationGliding);

	GetCharacterMovement()->AirControl = 0.05f;
	GetCharacterMovement()->MaxWalkSpeed = 4000.0f;
	GetCharacterMovement()->GravityScale = 1.0f;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
}

// Called to bind functionality to input
void ACharacterPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAxis("LookUp", this, &ACharacterPlayerBase::AddControllerPitchInput);
		PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterPlayerBase::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterPlayerBase::MoveRight);

		PlayerInputComponent->BindAction("EquipWeapon", EInputEvent::IE_Pressed, this, &ACharacterPlayerBase::EquipWeapon);
		PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACharacterPlayerBase::AimingOn);
		PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACharacterPlayerBase::AimingOff);
		PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ACharacterPlayerBase::Fire);

		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);

		PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &ACharacterPlayerBase::Interact);

		PlayerInputComponent->BindAction("TestFunction", EInputEvent::IE_Pressed, this, &ACharacterPlayerBase::TestFunction);
	}
}

void ACharacterPlayerBase::Shooting(void)
{
	if (HasAuthority() == true)return;
	
	//클라이언트
	//내 디바이스
	if (GetController() == UGameplayStatics::GetPlayerController(this, 0))
	{
		UGameplayStatics::GetPlayerCameraManager(this, 0)->PlayCameraShake(UCameraShakeRecoil::StaticClass());

		UGameplayStatics::SpawnEmitterAttached(rifleFlare, weaponHeld, "Muzzle",
			((FVector)(ForceInit)), FRotator::ZeroRotator, ((FVector)(1.0f)), EAttachLocation::SnapToTarget);

		UGameplayStatics::PlaySoundAtLocation(this, rifleSound, GetActorLocation());

		bool lineTraceHit = false;

		TArray<TEnumAsByte<EObjectTypeQuery>>objectTypes;
		objectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
		objectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		objectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Vehicle));
		objectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Destructible));
		objectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

		TArray<AActor *>actorsToIgnore;
		actorsToIgnore.Emplace(this);

		FHitResult outHit;
		FVector shootingDirection;
		shootingDirection = UKismetMathLibrary::GetForwardVector(GetControlRotation());

		lineTraceHit = UKismetSystemLibrary::LineTraceSingleForObjects(this, springArm->GetComponentLocation(),
			springArm->GetComponentLocation() + shootingDirection * 10000.0f,
			objectTypes, true, actorsToIgnore, EDrawDebugTrace::None, outHit, true);

		if (lineTraceHit == true)
		{
			ACharacterPlayerBase *hitPerson = Cast<ACharacterPlayerBase>(outHit.GetActor());

			FVector effectLocation;
			effectLocation = outHit.ImpactPoint + outHit.ImpactNormal*20.0;

			if (hitPerson)
			{
				if (outHit.GetComponent()->ComponentHasTag(FName("Body")) == true)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodEffect, effectLocation);

					TSubclassOf<UDamageType>damageTypeClass;

					Server_Damage(hitPerson, 10.0f, damageTypeClass, shootingDirection, outHit.BoneName);

					Server_ShotEffect(effectLocation, true);
				}
				else
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffect, effectLocation);

					Server_ShotEffect(effectLocation, false);
				}
			}
			else
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffect, effectLocation);

				Server_ShotEffect(effectLocation, false);
			}
		}
	}
	//남 디바이스
	else
	{
		UGameplayStatics::SpawnEmitterAttached(rifleFlare, weaponHeld, "Muzzle",
			((FVector)(ForceInit)), FRotator::ZeroRotator, ((FVector)(1.0f)), EAttachLocation::SnapToTarget);

		UGameplayStatics::PlaySoundAtLocation(this, rifleSound, GetActorLocation());
	}
}

bool ACharacterPlayerBase::Server_Damage_Validate(ACharacterPlayerBase * hitPerson, float damage, TSubclassOf<UDamageType> damageType, const FVector & hitFromDirection, const FName & boneName)
{
	return true;
}

void ACharacterPlayerBase::Server_Damage_Implementation(ACharacterPlayerBase * hitPerson, float damage, TSubclassOf<UDamageType> damageType, const FVector & hitFromDirection, const FName & boneName)
{
	hitPerson->Server_ApplyDamage(damage, damageType, hitFromDirection, boneName, GetController(), this);
}

bool ACharacterPlayerBase::Server_ApplyDamage_Validate(float damage, TSubclassOf<UDamageType> damageType, const FVector & hitFromDirection, const FName & boneName, AController * damageInstigator, AActor * damageCauser)
{
	return true;
}

void ACharacterPlayerBase::Server_ApplyDamage_Implementation(float damage, TSubclassOf<UDamageType> damageType, const FVector & hitFromDirection, const FName & boneName, AController * damageInstigator, AActor * damageCauser)
{
	if (dead == true)return;

	AGameStateBattleRoyal *currentGameState = nullptr;
	currentGameState = Cast<AGameStateBattleRoyal>(UGameplayStatics::GetGameState(this));

	if (currentGameState)
	{
		if (currentGameState->GetBattleBegun() == false)return;
	}

	hp -= damage;

	if (boneName.Compare(FName("Head")) == 0)
	{
		hp = 0.0f;
	}

	if (hp <= 0.0f)
	{
		Multicast_Die();

		dead = true;

		APlayerControllerBattleRoyal *controller = nullptr;
		controller = Cast<APlayerControllerBattleRoyal>(GetController());

		if (controller)
		{
			controller->KillPlayer();
		}
	}
	else
	{
		Multicast_Shot();
	}
}

bool ACharacterPlayerBase::Multicast_Die_Validate()
{
	return true;
}

void ACharacterPlayerBase::Multicast_Die_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);

	timelineShotRecovery->Stop();
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(FName("Hips"), 1.0f);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	weaponBack->SetSimulatePhysics(true);
	weaponHeld->SetSimulatePhysics(true);

	weaponBack->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	weaponHeld->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

bool ACharacterPlayerBase::Multicast_Shot_Validate()
{
	return true;
}

void ACharacterPlayerBase::Multicast_Shot_Implementation()
{
	GetMesh()->SetAllBodiesBelowSimulatePhysics("Spine", true);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight("Spine", 1.0f);

	timelineShotRecovery->PlayFromStart();
}

FVector ACharacterPlayerBase::GetMeshForwardVector(void)
{
	return GetMesh()->GetForwardVector();
}

FVector ACharacterPlayerBase::GetMeshLocation(void)
{
	return GetMesh()->GetComponentLocation();
}

void ACharacterPlayerBase::ElectricShock(float damage)
{
	APlayerControllerBattleRoyal *controller = nullptr;
	controller = Cast<APlayerControllerBattleRoyal>(GetController());

	if (IsValid(controller) == false)return;

	hp -= damage;

	if (hp <= 0.0f)
	{
		hp = 0.0f;

		Multicast_Die();

		dead = true;

		controller->KillPlayer();
	}
	else
	{
		controller->Client_PlayScreenEffectBlood();
	}
}

void ACharacterPlayerBase::ChangeFieldOfView(float fieldOfView)
{
	if (camera)
	{
		camera->FieldOfView = fieldOfView;
	}
}

void ACharacterPlayerBase::Shot(float physicsBlendWeight)
{
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight("Spine", physicsBlendWeight);
}

void ACharacterPlayerBase::Recover()
{
	GetMesh()->SetAllBodiesBelowSimulatePhysics("Spine", false);
}

bool ACharacterPlayerBase::SpawnBonfire_Validate()
{
	if (IsValid(this) == true)
	{
		return true;
	}
	return false;
}

void ACharacterPlayerBase::SpawnBonfire_Implementation()
{
	FTransform initialTransform;
	initialTransform = this->GetTransform();

	FVector location;
	location = this->GetActorLocation() + UKismetMathLibrary::GetForwardVector(this->GetActorRotation())*200.0f;

	initialTransform.SetLocation(location);
	
	FActorSpawnParameters spawnParameters;
	spawnParameters = FActorSpawnParameters();
	spawnParameters.Owner = GetController();

	GetWorld()->SpawnActor<AClassBonfire>(AClassBonfire::StaticClass(), initialTransform, spawnParameters);
}

void ACharacterPlayerBase::MoveForward(float value)
{
	moveForwardValue = value;

	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f))*value);

	if (previousMoveForwardValue != moveForwardValue)
	{
		Server_MoveForward(moveForwardValue);
	}

	previousMoveForwardValue = moveForwardValue;

	maximumDescendingPitch = moveForwardValue * 20.0f;
}

bool ACharacterPlayerBase::Server_MoveForward_Validate(float value)
{
	return true;
}

void ACharacterPlayerBase::Server_MoveForward_Implementation(float value)
{
	moveForwardValue = value;
}

bool ACharacterPlayerBase::Server_MoveRight_Validate(float value)
{
	return true;
}

void ACharacterPlayerBase::Server_MoveRight_Implementation(float value)
{
	moveRightValue = value;
}

bool ACharacterPlayerBase::Server_PickUpItem_Validate(AActorFieldItem * pickedItem)
{
	return true;
}

void ACharacterPlayerBase::Server_PickUpItem_Implementation(AActorFieldItem * pickedItem)
{
	if (pickedItem)
	{
		if (pickedItem->IsActorBeingDestroyed() == true)return;

		if (itemDataTable)
		{
			FString rowName;
			rowName = FString::FromInt((int32)pickedItem->GetItemClassID());

			FItemClass *tableRow;
			tableRow = itemDataTable->FindRow<FItemClass>(FName(*rowName), FString("Failed"));

			if (tableRow)
			{
				switch (tableRow->itemEquipmentType)
				{
				case EItemEquipmentType::Equipment_Upper:
					upperItemClassID = (int32)tableRow->itemClassID;
					break;
				case EItemEquipmentType::Equipment_Lower:
					lowerItemClassID = (int32)tableRow->itemClassID;
					break;
				case EItemEquipmentType::Equipment_Head:
					headItemClassID = (int32)tableRow->itemClassID;
					break;
				case EItemEquipmentType::Equipment_Feet:
					feetItemClassID = (int32)tableRow->itemClassID;
					break;
				case EItemEquipmentType::Equipment_Hands:
					weaponItemClassID = (int32)tableRow->itemClassID;
					break;
				}
			}
		}

		pickedItem->Destroy();
	}
}

void ACharacterPlayerBase::Interact(void)
{
	if (falling == true)
	{
		Server_SpreadParachute();
	}
	else
	{
		AActorFieldItem *foundItem = nullptr;

		foundItem = FindItemsSurrounding();

		if (foundItem)
		{
			Server_PickUpItem(foundItem);
		}
	}
}

void ACharacterPlayerBase::ArmDisarm()
{
	if (weaponEquipped == true)
	{
		weaponBack->SetStaticMesh(nullptr);
		weaponHeld->SetStaticMesh(mainWeapon);
	}
	else
	{
		weaponBack->SetStaticMesh(mainWeapon);
		weaponHeld->SetStaticMesh(nullptr);
	}
}

void ACharacterPlayerBase::ChangeAvatarUpper()
{
	if (upperItemClassID == 0)return;

	if (itemDataTable)
	{
		FString rowName;
		rowName = FString::FromInt(upperItemClassID);

		FItemClass *tableRow;
		tableRow = itemDataTable->FindRow<FItemClass>(FName(*rowName), FString("HeyYouFailedLoser"));

		if (tableRow)
		{
			bodyUpper->SetSkeletalMesh(tableRow->skeletalMesh);
		}
	}
}

void ACharacterPlayerBase::ChangeAvatarLower()
{
	if (lowerItemClassID == 0)return;

	if (itemDataTable)
	{
		FString rowName;
		rowName = FString::FromInt(lowerItemClassID);

		FItemClass *tableRow;
		tableRow = itemDataTable->FindRow<FItemClass>(FName(*rowName), FString("HeyYouFailedLoser"));

		if (tableRow)
		{
			bodyLower->SetSkeletalMesh(tableRow->skeletalMesh);
		}
	}
}

void ACharacterPlayerBase::ChangeAvatarHead()
{
	if (headItemClassID == 0)return;

	if (itemDataTable)
	{
		FString rowName;
		rowName = FString::FromInt(headItemClassID);

		FItemClass *tableRow;
		tableRow = itemDataTable->FindRow<FItemClass>(FName(*rowName), FString("HeyYouFailedLoser"));

		if (tableRow)
		{
			hair->SetSkeletalMesh(tableRow->skeletalMesh);
		}
	}
}

void ACharacterPlayerBase::ChangeAvatarFeet()
{
	if (feetItemClassID == 0)return;

	if (itemDataTable)
	{
		FString rowName;
		rowName = FString::FromInt(feetItemClassID);

		FItemClass *tableRow;
		tableRow = itemDataTable->FindRow<FItemClass>(FName(*rowName), FString("HeyYouFailedLoser"));

		if (tableRow)
		{
			feet->SetSkeletalMesh(tableRow->skeletalMesh);
		}
	}
}

void ACharacterPlayerBase::ChangeAvatarWeapon()
{
	if (weaponItemClassID == 0)return;

	if (itemDataTable)
	{
		FString rowName;
		rowName = FString::FromInt(weaponItemClassID);

		FItemClass *tableRow;
		tableRow = itemDataTable->FindRow<FItemClass>(FName(*rowName), FString("HeyYouFailedLoser"));

		if (tableRow)
		{
			mainWeapon = tableRow->staticMesh;
			carryingWeapon = true;

			if (weaponEquipped == true)
			{
				weaponBack->SetStaticMesh(nullptr);
				weaponHeld->SetStaticMesh(mainWeapon);
			}
			else
			{
				weaponBack->SetStaticMesh(mainWeapon);
				weaponHeld->SetStaticMesh(nullptr);
			}
		}
	}
}

bool ACharacterPlayerBase::Server_Fire_Validate()
{
	return true;
}

void ACharacterPlayerBase::Server_Fire_Implementation()
{
	Multicast_Fire();
}

bool ACharacterPlayerBase::Server_SetWeaponEquipped_Validate(bool equippedFlag)
{
	return true;
}

void ACharacterPlayerBase::Server_SetWeaponEquipped_Implementation(bool equippedFlag)
{
	weaponEquipped = equippedFlag;
}

bool ACharacterPlayerBase::Server_SetAiming_Validate(bool aimingFlag)
{
	return true;
}

void ACharacterPlayerBase::Server_SetAiming_Implementation(bool aimingFlag)
{
	aiming = aimingFlag;
}

bool ACharacterPlayerBase::Multicast_Fire_Validate()
{
	return true;
}

void ACharacterPlayerBase::Multicast_Fire_Implementation()
{
	if (HasAuthority() == true)return;

	//내 디바이스
	if (GetController() == UGameplayStatics::GetPlayerController(this, 0))return;

	GetMesh()->GetAnimInstance()->Montage_Play(animationFire);
}

bool ACharacterPlayerBase::Server_ShotEffect_Validate(FVector effectLocation, bool hitPlayer)
{
	return true;
}

void ACharacterPlayerBase::Server_ShotEffect_Implementation(FVector effectLocation, bool hitPlayer)
{
	Multicast_ShotEffect(effectLocation, hitPlayer);
}

bool ACharacterPlayerBase::Multicast_ShotEffect_Validate(FVector effectLocation, bool hitPlayer)
{
	return true;
}

void ACharacterPlayerBase::Multicast_ShotEffect_Implementation(FVector effectLocation, bool hitPlayer)
{
	if (HasAuthority() == true)return;

	if (GetController() == UGameplayStatics::GetPlayerController(this, 0))return;

	if (hitPlayer == true)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodEffect, effectLocation);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffect, effectLocation);
	}
}

void ACharacterPlayerBase::Die()
{
	GetMesh()->GetAnimInstance()->Montage_Play(animationDie);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void ACharacterPlayerBase::AddControllerPitchInput(float Val)
{
	APawn::AddControllerPitchInput(Val);

	waistPitch = GetControlRotation().Pitch;

	if (UKismetMathLibrary::Abs(waistPitch - previousWaistPitch) > 5.0f)
	{
		Server_UpdateWaistPitch(waistPitch);

		previousWaistPitch = waistPitch;
	}
}

bool ACharacterPlayerBase::Server_MotionFalling_Validate()
{
	return true;
}

void ACharacterPlayerBase::Server_MotionFalling_Implementation()
{
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = 10000.0f;
	GetCharacterMovement()->GravityScale = 0.5f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 10.0f, 0.0f);
}

bool ACharacterPlayerBase::Server_UpdateWaistPitch_Validate(float updatedValue)
{
	return true;
}

void ACharacterPlayerBase::Server_UpdateWaistPitch_Implementation(float updatedValue)
{
	waistPitch = updatedValue;
}

bool ACharacterPlayerBase::Server_SpreadParachute_Validate()
{
	return true;
}

void ACharacterPlayerBase::Server_SpreadParachute_Implementation()
{
	if ((falling == false) && (gliding == true))return;

	if (falling == true)falling = false;
	if (gliding == false)gliding = true;
}

bool ACharacterPlayerBase::Server_MotionGliding_Validate()
{
	return true;
}

void ACharacterPlayerBase::Server_MotionGliding_Implementation()
{
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = 4000.0f;
	GetCharacterMovement()->GravityScale = 0.1f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 10.0f, 0.0f);
	GetCharacterMovement()->Velocity *= 0.5f;

	UKismetSystemLibrary::PrintString(this, FString("Parachute sprrrrrrrrrrrread!!!!"), true, true, FLinearColor::Blue, 8.0f);
}

void ACharacterPlayerBase::CurveParachuteSpreadingTimeline(float parachuteSpreadedAmount)
{
	parachute->SetRelativeScale3D(FVector(parachuteSpreadedAmount));
}

void ACharacterPlayerBase::SequenceParachuteCode(void)
{
	FFindFloorResult outFloorResult;

	GetCharacterMovement()->ComputeFloorDist(GetCapsuleComponent()->GetComponentLocation(), 30000.0f, 30000.0f, outFloorResult, 34.0f);

	if (outFloorResult.FloorDist < 20000.0f)
	{
		parachuteCloud->Deactivate();

		if (outFloorResult.FloorDist < 500.0f)
		{
			Server_Landing();
		}
		else
		{
			if (gliding == true)
			{

			}
			else
			{
				Server_SpreadParachute();
			}
		}
	}
}

void ACharacterPlayerBase::SequenceDescendingRotationCode(void)
{
	currentDescendingPitch = UKismetMathLibrary::FInterpTo(currentDescendingPitch, maximumDescendingPitch, 0.3f, 0.05f);
	currentDescendingRoll = UKismetMathLibrary::FInterpTo(currentDescendingRoll, maximumDescendingRoll, 0.3f, 0.05f);

	GetMesh()->SetRelativeRotation(FRotator(currentDescendingRoll, -90.0f, currentDescendingPitch));
}

void ACharacterPlayerBase::MoveRight(float value)
{
	moveRightValue = value;

	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f))*value);

	if (previousMoveRightValue != moveRightValue)
	{
		Server_MoveRight(moveRightValue);
	}

	previousMoveRightValue = moveRightValue;

	maximumDescendingRoll = moveRightValue * 20.0f;
}

void ACharacterPlayerBase::EquipWeapon(void)
{
	if (carryingWeapon == false)return;

	Server_SetWeaponEquipped(!weaponEquipped);
}

void ACharacterPlayerBase::AimingOn(void)
{
	if (weaponEquipped == false)return;

	Server_SetAiming(true);

	aiming = true;

	if (crosshair)
	{
		crosshair->AddToViewport();
	}

	timelineCrosshair->Play();
}

void ACharacterPlayerBase::AimingOff(void)
{
	Server_SetAiming(false);

	aiming = false;

	if (crosshair)
	{
		crosshair->RemoveFromParent();
	}

	timelineCrosshair->Reverse();
}

void ACharacterPlayerBase::Fire(void)
{
	if ((carryingWeapon == false) || (weaponEquipped == false) || (aiming == false))return;

	GetMesh()->GetAnimInstance()->Montage_Play(animationFire);

	Server_Fire();
}

AActorFieldItem * ACharacterPlayerBase::FindItemsSurrounding(void)
{
	TArray<TEnumAsByte<EObjectTypeQuery>>objectTypes;
	objectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	TArray<AActor *>actorsToIgnore;

	TArray<AActor *>outActors;

	bool overlapped = false;
	overlapped = UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), 200.0f,
		objectTypes, AActorFieldItem::StaticClass(), actorsToIgnore, outActors);

	if (overlapped == true)
	{
		AActorFieldItem *currentOutActor = nullptr;
		int actorIndex = 0;

		bool foundByTracing = false;

		for (int32 i = 0; i < outActors.Num(); ++i)
		{
			currentOutActor = Cast<AActorFieldItem>(outActors[i]);

			if (currentOutActor)
			{
				FHitResult outHit;
				FCollisionQueryParams params;
				bool itemHit = false;

				itemHit = currentOutActor->GetSphere()->LineTraceComponent(outHit, springArm->GetComponentLocation(),
					UKismetMathLibrary::GetForwardVector(GetControlRotation())*1000.0f + springArm->GetComponentLocation(), params);

				if (itemHit == true)
				{
					return currentOutActor;
				}
			}
		}

		if (foundByTracing == false)
		{
			float distance = 99999999.9f;

			for (int32 i = 0; i < outActors.Num(); ++i)
			{
				if (UKismetMathLibrary::Subtract_VectorVector(GetActorLocation(), outActors[i]->GetActorLocation()).Size() < distance)
				{
					distance = UKismetMathLibrary::Subtract_VectorVector(GetActorLocation(), outActors[i]->GetActorLocation()).Size();
					actorIndex = i;

					foundByTracing = true;
				}
			}
		}

		if (foundByTracing == false)
		{
			return nullptr;
		}
		else
		{
			return Cast<AActorFieldItem>(outActors[actorIndex]);
		}
	}

	return nullptr;
}

void ACharacterPlayerBase::TestFunction(void)
{
	SpawnBonfire();
}

void ACharacterPlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterPlayerBase, moveForwardValue);
	DOREPLIFETIME(ACharacterPlayerBase, moveRightValue);
	DOREPLIFETIME(ACharacterPlayerBase, weaponEquipped);
	DOREPLIFETIME(ACharacterPlayerBase, aiming);

	DOREPLIFETIME(ACharacterPlayerBase, upperItemClassID);
	DOREPLIFETIME(ACharacterPlayerBase, lowerItemClassID);
	DOREPLIFETIME(ACharacterPlayerBase, headItemClassID);
	DOREPLIFETIME(ACharacterPlayerBase, feetItemClassID);
	DOREPLIFETIME(ACharacterPlayerBase, weaponItemClassID);

	DOREPLIFETIME(ACharacterPlayerBase, dead);
	DOREPLIFETIME(ACharacterPlayerBase, waistPitch);

	DOREPLIFETIME(ACharacterPlayerBase, hp);

	DOREPLIFETIME(ACharacterPlayerBase, falling);
	DOREPLIFETIME(ACharacterPlayerBase, gliding);
}
