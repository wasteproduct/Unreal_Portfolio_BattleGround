// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateBattleRoyal.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API AGameStateBattleRoyal : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AGameStateBattleRoyal();
	
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	void SetAlivePlayers(int32 alivePlayersNumber);
	void SetBattleBegun(bool newFlag);

	int32 GetAlivePlayers(void) { return alivePlayers; }
	bool GetBattleBegun(void) { return battleBegun; }

	FVector GetCurrentCircleLocation(void) { return currentCircleLocation; }
	FVector GetNextCircleLocation(void) { return nextCircleLocation; }
	float GetCurrentCircleRadius(void) { return currentCircleRadius; }
	float GetNextCircleRadius(void) { return nextCircleRadius; }

	bool GetShowingCurrentCircle(void) { return showingCurrentCircle; }
	bool GetShowingNextCircle(void) { return showingNextCircle; }

	void MainTimer_UpdateCircle(void);

protected:
	UFUNCTION()
		void ClearMessage();

private:
	void SetElectricFieldPhase(int32 phase, const FVector previousCircleLocation, float previousCircleRadius);

	void ElectricFieldPhaseNotification(const FName&message);

protected:
	UPROPERTY(Replicated)
		int32 alivePlayers;

	UPROPERTY(Replicated)
		bool battleBegun;

	UPROPERTY(Replicated)
		FVector currentCircleLocation;

	UPROPERTY(Replicated)
		FVector nextCircleLocation;

	UPROPERTY(Replicated)
		float currentCircleRadius;

	UPROPERTY(Replicated)
		float nextCircleRadius;

	UPROPERTY(Replicated)
		bool showingCurrentCircle;

	UPROPERTY(Replicated)
		bool showingNextCircle;

private:
	int32 waitingTime;
	int32 nextCircleExposureCount;
	int32 shrinkingTime;

	FVector deltaLocationPerSecond;
	float deltaRadiusPerSecond;

	UDataTable *electricFieldDataTable;

	int32 currentPhase;

	bool electricFieldPhaseNotificationMessageUp;
	bool electricFieldShrinkingNotificationMessageUp;
};
