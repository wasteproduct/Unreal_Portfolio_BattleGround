// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimationInstancePlayerBase.h"
#include "CharacterPlayerBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

UAnimationInstancePlayerBase::UAnimationInstancePlayerBase()
	:player(nullptr), moveForward(0.0f), moveRight(0.0f), legsRotation(0.0f), currentLegsRotation(0.0f), aiming(false),
	weaponEquipped(false), controllerPitch(0.0f), falling(false), previousFrameFalling(false), animationJump(nullptr),
	animationJumpEquipped(nullptr)
{
	bendingRotation.Pitch = 0.0f;
	bendingRotation.Yaw = 0.0f;

	//

	ConstructorHelpers::FObjectFinder<UAnimMontage>montageJump(TEXT("/Game/Characters/Female/Animations/Jog_Jump_Montage"));

	if (montageJump.Succeeded())
	{
		animationJump = montageJump.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>montageJumpEquipped(TEXT("/Game/Characters/Female/Animations/Rifle_Jump_Montage"));

	if (montageJumpEquipped.Succeeded())
	{
		animationJumpEquipped = montageJumpEquipped.Object;
	}
}

void UAnimationInstancePlayerBase::Update(float delta)
{
	if (IsValid(TryGetPawnOwner()) == true)
	{
		player = Cast<ACharacterPlayerBase>(TryGetPawnOwner());

		if (player)
		{
			aiming = player->GetAiming();
			weaponEquipped = player->GetWeaponEquipped();

			//

			if (aiming == true)
			{
				//controllerPitch = player->GetControlRotation().Pitch;
				controllerPitch = player->GetWaistPitch();
			}
			else
			{
				controllerPitch = 0.0f;
			}
			bendingRotation.Roll = -controllerPitch;

			//

			moveForward = player->GetMoveForward();
			moveRight = player->GetMoveRight();

			legsRotation = moveRight * 32.0f;

			if ((moveRight != 0.0f) && (moveForward == 0.0f))
			{
				moveForward = 1.0f;
			}

			legsRotation = moveForward * legsRotation;

			currentLegsRotation = UKismetMathLibrary::FInterpTo(currentLegsRotation, legsRotation, 1.0f, 0.2f);

			//

			Jump();
		}
	}
}

void UAnimationInstancePlayerBase::Shoot()
{
	if (IsValid(TryGetPawnOwner()))
	{
		ACharacterPlayerBase *player = Cast<ACharacterPlayerBase>(TryGetPawnOwner());

		if (player)
		{
			player->Shooting();
		}
	}
}

void UAnimationInstancePlayerBase::Jump(void)
{
	if ((player->GetFalling() == true) || (player->GetGliding() == true))return;

	falling = player->GetCharacterMovement()->IsFalling();

	if (falling == true && previousFrameFalling == false)
	{
		if (weaponEquipped == true)
		{
			Montage_Play(animationJumpEquipped);
		}
		else
		{
			Montage_Play(animationJump);
		}
	}
	else if (falling == true && previousFrameFalling == true)
	{

	}
	else if (falling == false && previousFrameFalling == true)
	{
		Montage_SetNextSection(FName("Loop"), FName("Landing"));
	}

	previousFrameFalling = falling;
}
