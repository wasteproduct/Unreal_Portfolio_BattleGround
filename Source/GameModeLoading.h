// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeLoading.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API AGameModeLoading : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGameModeLoading();
	
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;
};
