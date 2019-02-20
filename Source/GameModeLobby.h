// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeLobby.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API AGameModeLobby : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeLobby();

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;
};
