// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerLoading.h"
#include "UObject/ConstructorHelpers.h"
#include "UserWidgetLoading.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameMode.h"

APlayerControllerLoading::APlayerControllerLoading()
	:loading(nullptr)
{
	ConstructorHelpers::FClassFinder<UUserWidgetLoading>loadingObject(TEXT("/Game/GameModes/Loading/Blueprint_Widget_Loading"));

	if (loadingObject.Succeeded())
	{
		subclassWidgetLoading = loadingObject.Class;
	}
}

void APlayerControllerLoading::BeginPlay()
{
	Super::BeginPlay();

	if (subclassWidgetLoading)
	{
		loading = CreateWidget<UUserWidgetLoading>(this, subclassWidgetLoading);
	}

	if (loading)
	{
		loading->AddToViewport();
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(*UGameplayStatics::ParseOption(UGameplayStatics::GetGameMode(GetWorld())->OptionsString, FString("NextMap"))));
}

void APlayerControllerLoading::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
