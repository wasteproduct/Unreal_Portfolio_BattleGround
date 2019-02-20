// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeLoading.h"
#include "PlayerControllerLoading.h"

AGameModeLoading::AGameModeLoading()
{
	PlayerControllerClass = APlayerControllerLoading::StaticClass();
}

void AGameModeLoading::BeginPlay()
{
	Super::BeginPlay();
}

void AGameModeLoading::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
