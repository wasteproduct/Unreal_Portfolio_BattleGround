// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetInteraction.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API UUserWidgetInteraction : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUserWidgetInteraction(const FObjectInitializer&ObjectInitializer);
	
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry&MyGeometry, float InDeltaTime)override;

	void SetInteractionText(const FName&newText);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock *TextBlock_0;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock *TextBlock_1;
};
