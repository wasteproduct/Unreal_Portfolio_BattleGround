// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetTitle.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

UUserWidgetTitle::UUserWidgetTitle(const FObjectInitializer&ObjectInitializer)
	:Super(ObjectInitializer), Button_65(nullptr), Button_66(nullptr)
{

}

void UUserWidgetTitle::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_65)
	{
		Button_65->OnClicked.AddDynamic(this, &UUserWidgetTitle::StartGame);
	}

	if (Button_66)
	{
		Button_66->OnClicked.AddDynamic(this, &UUserWidgetTitle::ExitGame);
	}
}

void UUserWidgetTitle::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUserWidgetTitle::StartGame()
{
	UGameplayStatics::OpenLevel(this, TEXT("Lobby"));
}

void UUserWidgetTitle::ExitGame()
{
	
}
