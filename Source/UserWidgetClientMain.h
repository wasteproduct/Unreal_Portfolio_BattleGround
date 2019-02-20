// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetClientMain.generated.h"

class UTextBlock;
class AGameStateBattleRoyal;
class ACharacterPlayerBase;
class UProgressBar;
class APlayerControllerBattleRoyal;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API UUserWidgetClientMain : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUserWidgetClientMain(const FObjectInitializer&ObjectInitializer);
	
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry&MyGeometry, float InDeltaTime)override;

	void SetText(const FName&newText);

public:
	UWidgetAnimation * screenEffectBlood;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock *TextBlock_86;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock *TextBlock_13;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar *HP_Bar;

private:
	AGameStateBattleRoyal * gameState;

	FString alivePlayers;
	FName alivePlayersNumber;

	ACharacterPlayerBase *playerCharacter;
	APlayerControllerBattleRoyal *playerController;
};
