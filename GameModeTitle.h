// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeTitle.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API AGameModeTitle : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGameModeTitle();
	
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;
};
