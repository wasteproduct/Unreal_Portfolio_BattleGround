// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeBattleRoyal.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerControllerBattleRoyal.h"
#include "CharacterPlayerBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"
#include "GameStateBattleRoyal.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterC130.h"

AGameModeBattleRoyal::AGameModeBattleRoyal()
	:remainingTimeToStart(0), currentGameState(nullptr), remainingTimeToLobby(5), c130(nullptr)
{
	PlayerControllerClass = APlayerControllerBattleRoyal::StaticClass();

	DefaultPawnClass = ACharacterPlayerBase::StaticClass();

	GameStateClass = AGameStateBattleRoyal::StaticClass();
}

void AGameModeBattleRoyal::BeginPlay()
{
	Super::BeginPlay();

	remainingTimeToStart = configRemainingTimeToStart;

	TArray<AActor *>outActors;

	UGameplayStatics::GetAllActorsWithTag(this, TEXT("C130"), outActors);

	if (outActors.Num() > 0)
	{
		c130 = Cast<ACharacterC130>(outActors[0]);

		if (c130)
		{

		}
		else
		{
			UKismetSystemLibrary::PrintString(this, FString("C130 NOT found."), true, true, FLinearColor::Red, 30.0f);
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, FString("C130 NOT found."), true, true, FLinearColor::Red, 30.0f);
	}

	currentGameState = Cast<AGameStateBattleRoyal>(UGameplayStatics::GetGameState(this));

	GetWorld()->GetTimerManager().SetTimer(mainTimerHandle, this, &AGameModeBattleRoyal::MainTimer, 1.0f, true);
}

void AGameModeBattleRoyal::PostLogin(APlayerController * NewPlayer)
{
	if (playingState != EPlayingState::Waiting)return;

	AGameModeBase::PostLogin(NewPlayer);

	APlayerControllerBattleRoyal *newPlayerController = nullptr;
	newPlayerController = Cast<APlayerControllerBattleRoyal>(NewPlayer);

	if (newPlayerController)
	{
		alivePlayers.Emplace(newPlayerController);

		newPlayerController->SetNotificationFromServer(TEXT("Prepare for battle."));
	}
}

void AGameModeBattleRoyal::Logout(AController * Exiting)
{
	AGameModeBase::Logout(Exiting);

	APlayerControllerBattleRoyal *exitingPlayerController = nullptr;
	exitingPlayerController = Cast<APlayerControllerBattleRoyal>(Exiting);

	if (exitingPlayerController)
	{
		alivePlayers.Remove(exitingPlayerController);
	}
}

void AGameModeBattleRoyal::KillPlayer(APlayerControllerBattleRoyal * deadPlayer)
{
	if (deadPlayer)
	{
		deadPlayer->Client_DisplayResult(false, alivePlayers.Num());

		alivePlayers.Remove(deadPlayer);

		deadPlayers.Emplace(deadPlayer);
	}
}

void AGameModeBattleRoyal::ClearC130(void)
{
	for (int32 i = 0; i < alivePlayers.Num(); ++i)
	{
		if (alivePlayers[i]->GetJumpedOut() == true)continue;

		if (IsValid(alivePlayers[i]) == false)continue;

		alivePlayers[i]->Server_JumpOut();
	}
}

void AGameModeBattleRoyal::MainTimer(void)
{
	switch (playingState)
	{
	case EPlayingState::Waiting:
		if (alivePlayers.Num() >= 3)
		{
			if (remainingTimeToStart <= 0)
			{
				playingState = EPlayingState::Battle;

				if (currentGameState)
				{
					currentGameState->SetAlivePlayers(alivePlayers.Num());
					currentGameState->SetBattleBegun(true);
				}

				MessageToAll(TEXT("Let's roll."));

				for (int32 i = 0; i < alivePlayers.Num(); ++i)
				{
					if (IsValid(alivePlayers[i]) == false)continue;

					alivePlayers[i]->Server_BoardC130(c130);
				}

				c130->SetStarted(true);

				GetWorld()->GetTimerManager().SetTimer(mainTimerHandle, this, &AGameModeBattleRoyal::ClearMessage, 5.0f, false);
			}
			else
			{
				FString message;

				message += FString::FromInt((int32)remainingTimeToStart);
				message += FString(" seconds to start.");

				MessageToAll(FName(*message));
			}

			remainingTimeToStart--;
		}
		else
		{
			remainingTimeToStart = configRemainingTimeToStart;
			MessageToAll(TEXT("Waiting for other players..."));
		}
		break;
	case EPlayingState::Battle:
		if (IsValid(currentGameState) == true)
		{
			currentGameState->MainTimer_UpdateCircle();

			currentGameState->SetAlivePlayers(alivePlayers.Num());

			if (currentGameState->GetAlivePlayers() <= 1)
			{
				alivePlayers[0]->Client_DisplayResult(true, 1);

				playingState = EPlayingState::Ceremony;
			}
			else
			{
				if (currentGameState->GetShowingCurrentCircle() == true)
				{
					for (int32 i = 0; i < alivePlayers.Num(); ++i)
					{
						if (IsValid(alivePlayers[i]) == false)continue;

						ACharacterPlayerBase *alivePlayer = nullptr;
						alivePlayer = Cast<ACharacterPlayerBase>(alivePlayers[i]->GetPawn());

						if (IsValid(alivePlayer) == false)continue;

						FVector alivePlayerLocation = alivePlayer->GetMeshLocation();
						alivePlayerLocation.Z = 0.0f;

						FVector currentCircleLocation = currentGameState->GetCurrentCircleLocation();
						currentCircleLocation.Z = 0.0f;
						
						float distanceFromPlayerToElectricFieldCenter;
						distanceFromPlayerToElectricFieldCenter = FVector(alivePlayerLocation - currentCircleLocation).Size();
						
						if (distanceFromPlayerToElectricFieldCenter >= currentGameState->GetCurrentCircleRadius())
						{
							alivePlayer->ElectricShock(10.0f);
						}
					}
				}
			}
		}
		break;
	case EPlayingState::Ceremony:
		if (remainingTimeToLobby <= 0)
		{
			UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, mainTimerHandle);

			for (int32 i = 0; i < alivePlayers.Num(); ++i)
			{
				if (IsValid(alivePlayers[i]) == false)continue;

				alivePlayers[i]->Client_OutToLobby();
			}

			for (int32 i = 0; i < deadPlayers.Num(); ++i)
			{
				if (IsValid(deadPlayers[i]) == false)continue;

				deadPlayers[i]->Client_OutToLobby();
			}

			UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)), true, FString("listen"));
		}
		else
		{
			remainingTimeToLobby--;

			FString toLobby;

			toLobby = FString::FromInt((int32)remainingTimeToLobby) + FString(" seconds to back to lobby.");

			MessageToAll(FName(*toLobby));
		}
		break;
	}
}

void AGameModeBattleRoyal::ClearMessage()
{
	MessageToAll(TEXT(" "));

	GetWorld()->GetTimerManager().SetTimer(mainTimerHandle, this, &AGameModeBattleRoyal::MainTimer, 1.0f, true);
}

void AGameModeBattleRoyal::MessageToAll(const FName & message)
{
	for (int32 i = 0; i < alivePlayers.Num(); ++i)
	{
		if (IsValid(alivePlayers[i]) == false)continue;

		alivePlayers[i]->SetNotificationFromServer(message);
	}

	for (int32 i = 0; i < deadPlayers.Num(); ++i)
	{
		if (IsValid(deadPlayers[i]) == false)continue;

		deadPlayers[i]->SetNotificationFromServer(message);
	}
}
