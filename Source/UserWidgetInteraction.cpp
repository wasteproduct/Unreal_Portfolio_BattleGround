// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetInteraction.h"
#include "Components/TextBlock.h"
#include "Fonts/SlateFontInfo.h"

UUserWidgetInteraction::UUserWidgetInteraction(const FObjectInitializer&ObjectInitializer)
	:Super(ObjectInitializer), TextBlock_0(nullptr), TextBlock_1(nullptr)
{

}

void UUserWidgetInteraction::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (TextBlock_0)
	{
		TextBlock_0->SetText(FText::FromName(TEXT("F Key")));
	}
}

void UUserWidgetInteraction::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUserWidgetInteraction::SetInteractionText(const FName & newText)
{
	if (TextBlock_1)
	{
		TextBlock_1->SetText(FText::FromName(newText));
	}
}
