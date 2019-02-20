// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBattleRoyal.generated.h"

class APlayerControllerBattleRoyal;
class AGameStateBattleRoyal;
class ACharacterC130;

UENUM()
enum class EPlayingState :uint8
{
	Waiting,
	Battle,
	Ceremony
};

/**
 * 
 */
UCLASS(Config=Game)
class PRACTICEACTOR_API AGameModeBattleRoyal : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGameModeBattleRoyal();

	virtual void BeginPlay()override;
	
	virtual void PostLogin(APlayerController* NewPlayer)override;
	virtual void Logout(AController* Exiting)override;

	void MessageToAll(const FName&message);

	void KillPlayer(APlayerControllerBattleRoyal *deadPlayer);

	void ClearC130(void);

protected:
	UFUNCTION()
		void MainTimer();

	UFUNCTION()
		void ClearMessage();

protected:
	UPROPERTY(Config)
		int configRemainingTimeToStart;

private:
	TArray<APlayerControllerBattleRoyal *>alivePlayers;
	TArray<APlayerControllerBattleRoyal *>deadPlayers;

	FTimerHandle mainTimerHandle;

	EPlayingState playingState;

	int remainingTimeToStart;

	AGameStateBattleRoyal *currentGameState;

	int remainingTimeToLobby;

	ACharacterC130 *c130;
};
