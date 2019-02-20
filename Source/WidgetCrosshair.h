// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetCrosshair.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API UWidgetCrosshair : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWidgetCrosshair(const FObjectInitializer&ObjectInitializer);
	
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry&MyGeometry, float InDeltaTime)override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UWidget *Crosshair;
};
