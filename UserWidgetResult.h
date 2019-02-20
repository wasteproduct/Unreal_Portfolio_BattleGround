// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetResult.generated.h"

class UImage;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API UUserWidgetResult : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUserWidgetResult(const FObjectInitializer&ObjectInitializer);
	
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry&MyGeometry, float InDeltaTime)override;

	void SetResult(bool winner, const FName&rank);

protected:
	UFUNCTION()
		void OutToLobby();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage *Image_0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock *TextBlock_0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock *TextBlock_1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton *Lobby;
};
