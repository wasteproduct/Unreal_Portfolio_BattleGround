// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetClientMain.h"
#include "Components/TextBlock.h"
#include "GameStateBattleRoyal.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllerBattleRoyal.h"
#include "CharacterPlayerBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ProgressBar.h"
#include "WidgetAnimation.h"

UUserWidgetClientMain::UUserWidgetClientMain(const FObjectInitializer&ObjectInitializer)
	:Super(ObjectInitializer), TextBlock_86(nullptr), TextBlock_13(nullptr), gameState(nullptr), playerCharacter(nullptr), HP_Bar(nullptr),
	playerController(nullptr), screenEffectBlood(nullptr)
{
	
}

void UUserWidgetClientMain::NativeConstruct()
{
	Super::NativeConstruct();

	if (TextBlock_86)
	{
		TextBlock_86->SetJustification(ETextJustify::Center);
	}

	gameState = Cast<AGameStateBattleRoyal>(UGameplayStatics::GetGameState(this));

	if (gameState)
	{
		if (TextBlock_13)
		{
			TextBlock_13->SetText(FText::FromName(TEXT("1047479651")));
		}
	}

	playerController = Cast<APlayerControllerBattleRoyal>(GetOwningPlayer());

	//

	UProperty *properties = nullptr;

	properties = GetClass()->FindPropertyByName(TEXT("ScreenEffectBlood"));

	if (properties->GetClass() == UObjectProperty::StaticClass())
	{
		UObjectProperty *objectProperty = nullptr;
		objectProperty = Cast<UObjectProperty>(properties);

		if (objectProperty->PropertyClass == UWidgetAnimation::StaticClass())
		{
			UObject *object = nullptr;
			object = objectProperty->GetObjectPropertyValue_InContainer(this);

			UWidgetAnimation *widgetAnimation = nullptr;
			widgetAnimation = Cast<UWidgetAnimation>(object);

			if (IsValid(widgetAnimation) == true)
			{
				screenEffectBlood = widgetAnimation;
			}
		}
	}
}

void UUserWidgetClientMain::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (playerController)
	{
		playerCharacter = Cast<ACharacterPlayerBase>(playerController->GetPawn());
	}

	if ((IsValid(playerCharacter) == true) && (IsValid(HP_Bar) == true))
	{
		HP_Bar->SetPercent(UKismetMathLibrary::FClamp(playerCharacter->GetHP() / playerCharacter->GetMaximumHP(), 0.0f, 1.0f));
	}

	if (gameState)
	{
		alivePlayers = FString::FromInt(gameState->GetAlivePlayers());

		alivePlayersNumber = FName(*alivePlayers);

		if (TextBlock_13)
		{
			TextBlock_13->SetText(FText::FromName(alivePlayersNumber));
		}
	}
}

void UUserWidgetClientMain::SetText(const FName & newText)
{
	if (TextBlock_86)
	{
		TextBlock_86->SetText(FText::FromName(newText));
	}
}
