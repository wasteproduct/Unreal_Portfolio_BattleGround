// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetTitle.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API UUserWidgetTitle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUserWidgetTitle(const FObjectInitializer&ObjectInitializer);
	
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry&MyGeometry, float InDeltaTime)override;

protected:
	UFUNCTION()
		void StartGame();

	UFUNCTION()
		void ExitGame();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton *Button_65;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton *Button_66;
};
