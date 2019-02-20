// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerTitle.h"
#include "UserWidgetTitle.h"
#include "UObject/ConstructorHelpers.h"
#include "WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

APlayerControllerTitle::APlayerControllerTitle()
	:title(nullptr), bensoundEpic(nullptr)
{
	ConstructorHelpers::FClassFinder<UUserWidgetTitle>titleClass(TEXT("/Game/GameModes/Title/Blueprint_Widget_Title"));

	if (titleClass.Succeeded())
	{
		subclassWidgetTitle = titleClass.Class;
	}

	//

	ConstructorHelpers::FObjectFinder<USoundCue>bensoundEpicObject(TEXT("/Game/BGM/bensound-epic_Cue"));

	if (bensoundEpicObject.Succeeded())
	{
		bensoundEpic = bensoundEpicObject.Object;
	}
}

void APlayerControllerTitle::BeginPlay()
{
	if (bensoundEpic)
	{
		UGameplayStatics::PlaySound2D(this, bensoundEpic);
	}

	if (subclassWidgetTitle)
	{
		title = CreateWidget<UUserWidgetTitle>(this, subclassWidgetTitle);

		if (title)
		{
			title->AddToViewport();

			UWidgetBlueprintLibrary::SetInputMode_UIOnly(this, title);

			bShowMouseCursor = true;
		}
	}
}
