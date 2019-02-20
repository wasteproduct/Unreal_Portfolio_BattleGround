// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetLobby.generated.h"

class UButton;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API UUserWidgetLobby : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUserWidgetLobby(const FObjectInitializer&ObjectInitializer);
	
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry&MyGeometry, float InDeltaTime)override;

protected:
	UFUNCTION()
		void JoinServer();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton *Button_52;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UEditableTextBox *EditableTextBox_181;
};
