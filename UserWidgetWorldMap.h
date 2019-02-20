// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetWorldMap.generated.h"

class UImage;
class APlayerControllerBattleRoyal;
class UTexture2D;
class UUserWidgetCircle;
class AGameStateBattleRoyal;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API UUserWidgetWorldMap : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUserWidgetWorldMap(const FObjectInitializer&ObjectInitializer);
	
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry&MyGeometry, float InDeltaTime)override;

private:
	FVector2D ConvertCoordinatesFromWorldToWidget(const FVector&worldLocation);
	float ConvertLengthFromWorldToWidget(const float&worldLength);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage * MyIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UUserWidgetCircle *CurrentCircle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UUserWidgetCircle *NextCircle;

private:
	APlayerControllerBattleRoyal * playerController;

	UTexture2D *c130Icon;
	UTexture2D *myIcon;

	AGameStateBattleRoyal *gameState;
};
