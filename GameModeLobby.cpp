// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeLobby.h"
#include "PlayerControllerLobby.h"

AGameModeLobby::AGameModeLobby()
{
	PlayerControllerClass = APlayerControllerLobby::StaticClass();

	DefaultPawnClass = nullptr;
}

void AGameModeLobby::BeginPlay()
{
	Super::BeginPlay();
}

void AGameModeLobby::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
