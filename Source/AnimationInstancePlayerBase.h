// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimationInstancePlayerBase.generated.h"

class ACharacterPlayerBase;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API UAnimationInstancePlayerBase : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UAnimationInstancePlayerBase();

	UFUNCTION(BlueprintCallable)
		void Update(float delta);

	UFUNCTION(BlueprintCallable)
		void Shoot();

private:
	void Jump(void);

protected:
	UPROPERTY(BlueprintReadOnly)
		float moveForward;
	UPROPERTY(BlueprintReadOnly)
		float moveRight;
	UPROPERTY(BlueprintReadOnly)
		float legsRotation;
	UPROPERTY(BlueprintReadOnly)
		float currentLegsRotation;
	UPROPERTY(BlueprintReadOnly)
		bool aiming;
	UPROPERTY(BlueprintReadOnly)
		bool weaponEquipped;
	UPROPERTY(BlueprintReadOnly)
		float controllerPitch;
	UPROPERTY(BlueprintReadOnly)
		FRotator bendingRotation;

	UPROPERTY(BlueprintReadOnly)
		bool falling;
	UPROPERTY(BlueprintReadOnly)
		bool previousFrameFalling;

private:
	ACharacterPlayerBase * player;

	UAnimMontage *animationJump;
	UAnimMontage *animationJumpEquipped;
};
