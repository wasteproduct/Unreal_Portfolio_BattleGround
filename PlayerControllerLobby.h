// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerLobby.generated.h"

class ACameraActor;
class USoundCue;
class UUserWidget;
class UUserWidgetLobby;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API APlayerControllerLobby : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerLobby();

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	void JoinServer(const FString&IPAddress);

private:
	ACameraActor * lobbyCamera;
	USoundCue *bensoundEpic;

	TSubclassOf<UUserWidget>subclassWidgetLobby;
	UUserWidgetLobby *lobby;
};
