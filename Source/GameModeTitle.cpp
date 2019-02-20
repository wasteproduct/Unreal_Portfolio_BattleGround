// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeTitle.h"
#include "PlayerControllerTitle.h"

AGameModeTitle::AGameModeTitle()
{
	PlayerControllerClass = APlayerControllerTitle::StaticClass();
}

void AGameModeTitle::BeginPlay()
{
	Super::BeginPlay();
}

void AGameModeTitle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
