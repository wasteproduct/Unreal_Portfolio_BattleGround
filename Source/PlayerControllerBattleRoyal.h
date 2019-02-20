// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBattleRoyal.generated.h"

class UUserWidget;
class UUserWidgetClientMain;
class ACharacterC130;
class UUserWidgetWorldMap;
class APawnFlyingViewer;
class ACharacterPlayerBase;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API APlayerControllerBattleRoyal : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerControllerBattleRoyal();

	virtual void BeginPlay()override;

	virtual void SetupInputComponent()override;

	void SetNotificationFromServer(const FName&newText);

	void KillPlayer(void);

	bool GetJumpedOut(void) { return jumpedOut; }

	FVector GetPawnLocation(void);
	bool InC130(void);
	FVector GetPawnForwardVector(void);

public:
	UFUNCTION(Client, Reliable, WithValidation)
		void Client_DisplayResult(bool winner, int32 rank);

	UFUNCTION(Client, Reliable, WithValidation)
		void Client_OutToLobby();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_BoardC130(ACharacterC130 *c130);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_JumpOut();

	UFUNCTION(Client, Unreliable, WithValidation)
		void Client_PlayScreenEffectBlood();

protected:
	UFUNCTION()
		void SetMainWidgetText();

	UFUNCTION()
		void WorldMapOnOff();

private:
	bool BaseCharacter(void);

protected:
	UPROPERTY(ReplicatedUsing = SetMainWidgetText)
		FName notificationFromServer;

	UPROPERTY()
		UUserWidgetClientMain *clientMain;

	UPROPERTY()
		UUserWidgetWorldMap *worldMap;

private:
	TSubclassOf<UUserWidget>subclassWidgetClientMain;

	TSubclassOf<UUserWidget>subclassWidgetResult;

	bool jumpedOut;

	TSubclassOf<UUserWidget>subclassWidgetWorldMap;
	bool mapOpened;
	APawnFlyingViewer *flyingViewer;

	ACharacterC130 *c130;
	ACharacterPlayerBase *baseCharacter;
};
