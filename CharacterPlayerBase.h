// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterPlayerBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UStaticMesh;
class UAnimMontage;
class USoundBase;
class UParticleSystem;
class UWidgetCrosshair;
class UTimelineComponent;
class UCurveFloat;
class UUserWidget;
class UDataTable;
class UWidgetComponent;
class AActorFieldItem;
class UParticleSystemComponent;
class UAudioComponent;

UCLASS()
class PRACTICEACTOR_API ACharacterPlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterPlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetMoveForward(void) { return moveForwardValue; }
	float GetMoveRight(void) { return moveRightValue; }

	bool GetAiming(void) { return aiming; }
	bool GetWeaponEquipped(void) { return weaponEquipped; }

	void Shooting(void);

	float GetWaistPitch(void) { return waistPitch; }

	float GetMaximumHP(void) { return maximumHP; }
	float GetHP(void) { return hp; }

	void SetFalling(bool flag) { falling = flag; }

	bool GetFalling(void) { return falling; }
	bool GetGliding(void) { return gliding; }

	FVector GetMeshForwardVector(void);
	FVector GetMeshLocation(void);

	void ElectricShock(float damage);

protected:
	UFUNCTION()
		void ChangeFieldOfView(float fieldOfView);

	UFUNCTION()
		void Shot(float physicsBlendWeight);

	UFUNCTION()
		void Recover();

	UFUNCTION(Server, Reliable, WithValidation)
		void SpawnBonfire();

	UFUNCTION(Server, Unreliable, WithValidation)
		void Server_MoveForward(float value);

	UFUNCTION(Server, Unreliable, WithValidation)
		void Server_MoveRight(float value);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_PickUpItem(AActorFieldItem *pickedItem);

	UFUNCTION()
		void ArmDisarm();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetWeaponEquipped(bool equippedFlag);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetAiming(bool aimingFlag);

	UFUNCTION()
		void ChangeAvatarUpper();

	UFUNCTION()
		void ChangeAvatarLower();

	UFUNCTION()
		void ChangeAvatarHead();

	UFUNCTION()
		void ChangeAvatarFeet();

	UFUNCTION()
		void ChangeAvatarWeapon();

	UFUNCTION(Server, Unreliable, WithValidation)
		void Server_Fire();

	UFUNCTION(NetMulticast, Unreliable, WithValidation)
		void Multicast_Fire();

	UFUNCTION(Server, Unreliable, WithValidation)
		void Server_ShotEffect(FVector effectLocation, bool hitPlayer);

	UFUNCTION(NetMulticast, Unreliable, WithValidation)
		void Multicast_ShotEffect(FVector effectLocation, bool hitPlayer);

	UFUNCTION(Server, Unreliable, WithValidation)
		void Server_Damage(ACharacterPlayerBase *hitPerson, float damage,
			TSubclassOf<UDamageType>damageType, const FVector&hitFromDirection, const FName&boneName);

	UFUNCTION(Server, Unreliable, WithValidation)
		void Server_ApplyDamage(float damage, TSubclassOf<UDamageType>damageType, const FVector&hitFromDirection,
			const FName&boneName, AController *damageInstigator, AActor *damageCauser);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_Die();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_Shot();

	UFUNCTION()
		void Die();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "up down addpitch"))
		virtual void AddControllerPitchInput(float Val)override;

	UFUNCTION(Server, Unreliable, WithValidation)
		void Server_UpdateWaistPitch(float updatedValue);

	UFUNCTION()
		void PlayAnimationFalling();

	UFUNCTION()
		void PlayAnimationGliding();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_MotionFalling();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SpreadParachute();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_MotionGliding();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Landing();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_StartWalking();

	UFUNCTION()
		void CurveParachuteSpreadingTimeline(float parachuteSpreadedAmount);

private:
	void SequenceParachuteCode(void);
	void SequenceDescendingRotationCode(void);

	void MoveRight(float value);
	void MoveForward(float value);
	void EquipWeapon(void);
	void AimingOn(void);
	void AimingOff(void);
	void Fire(void);

	void Interact(void);

	AActorFieldItem *FindItemsSurrounding(void);

	void StartWalking(void);

	void SpreadParachute(void);

	void TestFunction(void);

private:
	UPROPERTY()
		UWidgetCrosshair *crosshair;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent *bodyUpper;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent *bodyLower;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent *feet;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent *hair;

	UPROPERTY(Replicated)
		float moveForwardValue;

	UPROPERTY(Replicated)
		float moveRightValue;

	UPROPERTY(ReplicatedUsing = ArmDisarm)
		bool weaponEquipped;

	UPROPERTY(Replicated)
		bool aiming;

	UPROPERTY(ReplicatedUsing = ChangeAvatarUpper)
		int32 upperItemClassID;

	UPROPERTY(ReplicatedUsing = ChangeAvatarLower)
		int32 lowerItemClassID;

	UPROPERTY(ReplicatedUsing = ChangeAvatarHead)
		int32 headItemClassID;

	UPROPERTY(ReplicatedUsing = ChangeAvatarFeet)
		int32 feetItemClassID;

	UPROPERTY(ReplicatedUsing = ChangeAvatarWeapon)
		int32 weaponItemClassID;

	UPROPERTY(ReplicatedUsing = Die)
		bool dead;

	UPROPERTY(Replicated)
		float waistPitch;

	UPROPERTY(Replicated)
		float hp;

	UPROPERTY(ReplicatedUsing = PlayAnimationFalling)
		bool falling;

	UPROPERTY(ReplicatedUsing = PlayAnimationGliding)
		bool gliding;

private:
	USpringArmComponent *springArm;
	UCameraComponent *camera;

	UStaticMeshComponent *weaponBack;
	UStaticMeshComponent *weaponHeld;

	UStaticMesh *mainWeapon;
	UAnimMontage *animationFire;
	USoundBase *rifleSound;
	UParticleSystem *rifleFlare;
	TSubclassOf<UUserWidget>subclassCrosshairWidget;

	UTimelineComponent *timelineCrosshair;
	UCurveFloat *curveFieldOfView;

	UParticleSystem *bulletEffect;
	UParticleSystem *bloodEffect;

	float maximumHP;

	UTimelineComponent *timelineShotRecovery;
	UCurveFloat *curvePhysicsBlend;

	UDataTable *itemDataTable;

	UWidgetComponent *widgetInteraction;

	AActorFieldItem *fieldItem;

	bool carryingWeapon;

	float previousMoveForwardValue;
	float previousMoveRightValue;

	UAnimMontage *animationDie;

	float previousWaistPitch;

	UAnimMontage *animationFalling;
	UAnimMontage *animationGliding;

	UStaticMeshComponent *parachute;

	UTimelineComponent *timelineSpreadingParachute;
	UCurveFloat *curveSpreadingParachute;

	float maximumDescendingPitch;
	float maximumDescendingRoll;
	float currentDescendingPitch;
	float currentDescendingRoll;

	UParticleSystemComponent *parachuteCloud;
	UAudioComponent *windCue;
	USoundBase *soundSpreadingParachute;
};
