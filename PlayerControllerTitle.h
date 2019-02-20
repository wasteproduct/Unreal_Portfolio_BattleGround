// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerTitle.generated.h"

class UUserWidget;
class UUserWidgetTitle;
class USoundCue;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API APlayerControllerTitle : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerControllerTitle();
	
	virtual void BeginPlay()override;

private:
	TSubclassOf<UUserWidget>subclassWidgetTitle;
	UUserWidgetTitle *title;
	USoundCue *bensoundEpic;
};
