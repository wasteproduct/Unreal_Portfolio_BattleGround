// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerLoading.generated.h"

class UUserWidget;
class UUserWidgetLoading;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API APlayerControllerLoading : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerControllerLoading();
	
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

private:
	TSubclassOf<UUserWidget>subclassWidgetLoading;
	UUserWidgetLoading *loading;
};
