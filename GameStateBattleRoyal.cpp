// Fill out your copyright notice in the Description page of Project Settings.

#include "GameStateBattleRoyal.h"
#include "UnrealNetwork.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "ClassElectricFieldPhase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameModeBattleRoyal.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

AGameStateBattleRoyal::AGameStateBattleRoyal()
	:alivePlayers(0), battleBegun(false), showingCurrentCircle(false), showingNextCircle(false), waitingTime(0),
	nextCircleExposureCount(0), shrinkingTime(0), electricFieldDataTable(nullptr), currentPhase(0),
	electricFieldPhaseNotificationMessageUp(false), electricFieldShrinkingNotificationMessageUp(false)
{
	bAlwaysRelevant = true;

	//

	ConstructorHelpers::FObjectFinder<UDataTable>tableObject(TEXT("/Game/GameModes/Battle/Table_ElectricFieldPhase"));

	if (tableObject.Succeeded())
	{
		electricFieldDataTable = tableObject.Object;
	}
}

void AGameStateBattleRoyal::BeginPlay()
{
	Super::BeginPlay();

	SetElectricFieldPhase(1, nextCircleLocation, nextCircleRadius);
}

void AGameStateBattleRoyal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameStateBattleRoyal::SetAlivePlayers(int32 alivePlayersNumber)
{
	alivePlayers = alivePlayersNumber;
}

void AGameStateBattleRoyal::SetBattleBegun(bool newFlag)
{
	battleBegun = newFlag;
}

void AGameStateBattleRoyal::MainTimer_UpdateCircle(void)
{
	waitingTime--;

	if (waitingTime < 0)
	{
		showingNextCircle = true;

		nextCircleExposureCount--;

		if (nextCircleExposureCount < 0)
		{
			showingCurrentCircle = true;

			shrinkingTime--;

			if (shrinkingTime < 0)
			{
				showingNextCircle = false;

				SetElectricFieldPhase(currentPhase + 1, nextCircleLocation, nextCircleRadius);
			}
			else
			{
				currentCircleLocation += deltaLocationPerSecond;
				currentCircleRadius += deltaRadiusPerSecond;

				if (electricFieldShrinkingNotificationMessageUp == true)return;

				ElectricFieldPhaseNotification(TEXT("The electric field begins to shrink."));

				electricFieldShrinkingNotificationMessageUp = true;
			}
		}
		else
		{
			if (electricFieldPhaseNotificationMessageUp == true)return;

			ElectricFieldPhaseNotification(TEXT("A new electric field has been set."));

			electricFieldPhaseNotificationMessageUp = true;
		}
	}
}

void AGameStateBattleRoyal::SetElectricFieldPhase(int32 phase, const FVector previousCircleLocation, float previousCircleRadius)
{
	if (IsValid(electricFieldDataTable) == false)return;

	electricFieldPhaseNotificationMessageUp = false;
	electricFieldShrinkingNotificationMessageUp = false;

	FElectricFieldPhase *electricFieldPhase = nullptr;
	electricFieldPhase = electricFieldDataTable->FindRow<FElectricFieldPhase>(FName(*FString::FromInt(phase)), FString("Failed"));

	if (electricFieldPhase)
	{
		currentPhase = electricFieldPhase->phase;
		currentCircleLocation = electricFieldPhase->currentCircleLocation;
		currentCircleRadius = electricFieldPhase->currentCircleRadius;
		nextCircleLocation = electricFieldPhase->nextCircleLocation;
		nextCircleRadius = electricFieldPhase->nextCircleRadius;
		waitingTime = electricFieldPhase->waitingCount;
		nextCircleExposureCount = electricFieldPhase->nextCircleExposureCount;
		shrinkingTime = electricFieldPhase->shrinkingCount;

		if (currentPhase < 1)showingCurrentCircle = false;
		showingNextCircle = false;

		if (electricFieldPhase->randomPhase == true)
		{
			currentCircleLocation = previousCircleLocation;
			currentCircleRadius = previousCircleRadius;
			
			nextCircleLocation = currentCircleLocation + UKismetMathLibrary::RandomUnitVector()*UKismetMathLibrary::RandomFloatInRange(electricFieldPhase->minimumDistanceToNextCircle, electricFieldPhase->maximumDistanceToNextCircle);
		}

		deltaLocationPerSecond = (nextCircleLocation - currentCircleLocation) / (float)shrinkingTime;
		deltaRadiusPerSecond = (nextCircleRadius - currentCircleRadius) / (float)shrinkingTime;
	}
}

void AGameStateBattleRoyal::ElectricFieldPhaseNotification(const FName & message)
{
	AGameModeBattleRoyal *gameMode = nullptr;
	gameMode = Cast<AGameModeBattleRoyal>(UGameplayStatics::GetGameMode(this));

	if (IsValid(gameMode) == false)return;

	gameMode->MessageToAll(message);

	FTimerHandle clearMessage;
	GetWorld()->GetTimerManager().SetTimer(clearMessage, this, &AGameStateBattleRoyal::ClearMessage, 5.0f);
}

void AGameStateBattleRoyal::ClearMessage(void)
{
	AGameModeBattleRoyal *gameMode = nullptr;
	gameMode = Cast<AGameModeBattleRoyal>(UGameplayStatics::GetGameMode(this));

	if (IsValid(gameMode) == false)return;

	gameMode->MessageToAll(TEXT(" "));
}

void AGameStateBattleRoyal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameStateBattleRoyal, alivePlayers);
	DOREPLIFETIME(AGameStateBattleRoyal, battleBegun);

	DOREPLIFETIME(AGameStateBattleRoyal, currentCircleLocation);
	DOREPLIFETIME(AGameStateBattleRoyal, currentCircleRadius);
	DOREPLIFETIME(AGameStateBattleRoyal, nextCircleLocation);
	DOREPLIFETIME(AGameStateBattleRoyal, nextCircleRadius);

	DOREPLIFETIME(AGameStateBattleRoyal, showingCurrentCircle);
	DOREPLIFETIME(AGameStateBattleRoyal, showingNextCircle);
}
