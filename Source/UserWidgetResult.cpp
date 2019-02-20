// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetResult.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

UUserWidgetResult::UUserWidgetResult(const FObjectInitializer&ObjectInitializer)
	:Super(ObjectInitializer), Image_0(nullptr), TextBlock_0(nullptr), TextBlock_1(nullptr), Lobby(nullptr)
{

}

void UUserWidgetResult::NativeConstruct()
{
	Super::NativeConstruct();

	if (Lobby)
	{
		Lobby->OnClicked.AddDynamic(this, &UUserWidgetResult::OutToLobby);
	}
}

void UUserWidgetResult::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUserWidgetResult::SetResult(bool winner, const FName & rank)
{
	if (TextBlock_0)
	{
		if (winner == true)
		{
			TextBlock_0->SetText(FText::FromName(TEXT("You win. You're the best survivor.")));
		}
		else
		{
			TextBlock_0->SetText(FText::FromName(TEXT("You've lost. Your rank is... ")));
		}
	}
	
	if (TextBlock_1)
	{
		TextBlock_1->SetText(FText::FromName(rank));
	}
}

void UUserWidgetResult::OutToLobby()
{
	UGameplayStatics::OpenLevel(this, TEXT("Lobby"));
}
