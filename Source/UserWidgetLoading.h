// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetLoading.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API UUserWidgetLoading : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUserWidgetLoading(const FObjectInitializer&ObjectInitializer);
	
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry&MyGeometry, float InDeltaTime)override;
};
