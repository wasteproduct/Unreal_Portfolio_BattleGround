// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetLobby.h"
#include "Components/Button.h"
#include "PlayerControllerLobby.h"
#include "Kismet/GameplayStatics.h"
#include "Components/EditableTextBox.h"

UUserWidgetLobby::UUserWidgetLobby(const FObjectInitializer&ObjectInitializer)
	:Super(ObjectInitializer), Button_52(nullptr), EditableTextBox_181(nullptr)
{

}

void UUserWidgetLobby::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_52)
	{
		Button_52->OnClicked.AddDynamic(this, &UUserWidgetLobby::JoinServer);
	}
}

void UUserWidgetLobby::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUserWidgetLobby::JoinServer(void)
{
	APlayerControllerLobby *playerController = nullptr;
	playerController = Cast<APlayerControllerLobby>(UGameplayStatics::GetPlayerController(this, 0));

	if (playerController)
	{
		if (EditableTextBox_181)
		{
			bool stop;
			stop = true;
			playerController->JoinServer(EditableTextBox_181->GetText().ToString());
		}
	}
}
