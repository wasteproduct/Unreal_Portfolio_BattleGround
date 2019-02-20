// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetWorldMap.h"
#include "PlayerControllerBattleRoyal.h"
#include "Image.h"
#include "WidgetLayoutLibrary.h"
#include "CanvasPanelSlot.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "UserWidgetCircle.h"
#include "GameStateBattleRoyal.h"
#include "Kismet/GameplayStatics.h"

UUserWidgetWorldMap::UUserWidgetWorldMap(const FObjectInitializer&ObjectInitializer)
	:Super(ObjectInitializer), MyIcon(nullptr), playerController(nullptr), c130Icon(nullptr), myIcon(nullptr), CurrentCircle(nullptr),
	NextCircle(nullptr), gameState(nullptr)
{
	ConstructorHelpers::FObjectFinder<UTexture2D>c130IconTexture(TEXT("/Game/WorldMap/T_C130Icon"));

	if (c130IconTexture.Succeeded())
	{
		c130Icon = c130IconTexture.Object;
	}

	ConstructorHelpers::FObjectFinder<UTexture2D>myIconTexture(TEXT("/Game/WorldMap/T_MyIcon"));

	if (myIconTexture.Succeeded())
	{
		myIcon = myIconTexture.Object;
	}
}

void UUserWidgetWorldMap::NativeConstruct()
{
	Super::NativeConstruct();

	playerController = Cast<APlayerControllerBattleRoyal>(GetOwningPlayer());

	if (IsValid(CurrentCircle) == true)
	{
		CurrentCircle->SetCircleColor(FLinearColor::Blue);
	}
	if (IsValid(NextCircle) == true)
	{
		NextCircle->SetCircleColor(FLinearColor::White);
	}

	gameState = Cast<AGameStateBattleRoyal>(UGameplayStatics::GetGameState(this));
}

void UUserWidgetWorldMap::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(playerController) == true)
	{
		if (IsValid(MyIcon) == true)
		{
			if (playerController->InC130() == true)
			{
				if (c130Icon)
				{
					MyIcon->SetBrushFromTexture(c130Icon, true);
				}
			}
			else
			{
				if (myIcon)
				{
					MyIcon->SetBrushFromTexture(myIcon, true);
				}
			}

			UWidgetLayoutLibrary::SlotAsCanvasSlot(MyIcon)->SetPosition(ConvertCoordinatesFromWorldToWidget(playerController->GetPawnLocation()));

			float crossVectorZ;
			crossVectorZ = UKismetMathLibrary::Cross_VectorVector(FVector(0.0f, -1.0f, 0.0f), playerController->GetPawnForwardVector()).Z;
			crossVectorZ /= UKismetMathLibrary::Abs(crossVectorZ);

			float angle;
			angle = UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector(FVector(0.0f, -1.0f, 0.0f), playerController->GetPawnForwardVector()))*crossVectorZ;
			
			MyIcon->SetRenderAngle(angle + 90.0f);

			if (IsValid(gameState) == true)
			{
				if (IsValid(CurrentCircle) == true)
				{
					if (gameState->GetShowingCurrentCircle() == true)
					{
						CurrentCircle->SetVisibility(ESlateVisibility::Visible);
					}
					else
					{
						CurrentCircle->SetVisibility(ESlateVisibility::Collapsed);
					}

					CurrentCircle->SetLocationRadius(ConvertCoordinatesFromWorldToWidget(gameState->GetCurrentCircleLocation()), ConvertLengthFromWorldToWidget(gameState->GetCurrentCircleRadius()));
				}
				if (IsValid(NextCircle) == true)
				{
					if (gameState->GetShowingNextCircle() == true)
					{
						NextCircle->SetVisibility(ESlateVisibility::Visible);
					}
					else
					{
						NextCircle->SetVisibility(ESlateVisibility::Collapsed);
					}

					NextCircle->SetLocationRadius(ConvertCoordinatesFromWorldToWidget(gameState->GetNextCircleLocation()), ConvertLengthFromWorldToWidget(gameState->GetNextCircleRadius()));
				}
			}
		}
	}
}

FVector2D UUserWidgetWorldMap::ConvertCoordinatesFromWorldToWidget(const FVector & worldLocation)
{
	return FVector2D((worldLocation.X + 408000.0f) / 816000.0f*1024.0f, (worldLocation.Y + 408000.0f) / 816000.0f*1024.0f);
}

float UUserWidgetWorldMap::ConvertLengthFromWorldToWidget(const float & worldLength)
{
	float result = worldLength;
	result /= 1024.0f;

	return result;
}
