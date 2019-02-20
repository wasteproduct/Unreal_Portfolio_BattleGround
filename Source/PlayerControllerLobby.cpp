// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerLobby.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
#include "UserWidgetLobby.h"
#include "WidgetBlueprintLibrary.h"

APlayerControllerLobby::APlayerControllerLobby()
	:lobbyCamera(nullptr), bensoundEpic(nullptr), lobby(nullptr)
{
	ConstructorHelpers::FObjectFinder<USoundCue>bensoundEpicObject(TEXT("/Game/BGM/bensound-epic_Cue"));

	if (bensoundEpicObject.Succeeded())
	{
		bensoundEpic = bensoundEpicObject.Object;
	}

	//

	ConstructorHelpers::FClassFinder<UUserWidgetLobby>lobbyClass(TEXT("/Game/GameModes/Lobby/Blueprint_Widget_Lobby"));

	if (lobbyClass.Succeeded())
	{
		subclassWidgetLobby = lobbyClass.Class;
	}
}

void APlayerControllerLobby::BeginPlay()
{
	Super::BeginPlay();

	if (bensoundEpic)
	{
		UGameplayStatics::PlaySound2D(this, bensoundEpic);
	}

	//

	TArray<AActor *>outActors;
	
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("LobbyCamera"), outActors);

	if (outActors.Num() > 0)
	{
		lobbyCamera = Cast<ACameraActor>(outActors[0]);
	}

	if (lobbyCamera)
	{
		SetViewTargetWithBlend(lobbyCamera);
	}

	//

	if (subclassWidgetLobby)
	{
		lobby = CreateWidget<UUserWidgetLobby>(this, subclassWidgetLobby);
	}

	if (lobby)
	{
		lobby->AddToViewport();

		UWidgetBlueprintLibrary::SetInputMode_UIOnly(this, lobby);

		bShowMouseCursor = true;
	}
}

void APlayerControllerLobby::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerControllerLobby::JoinServer(const FString & IPAddress)
{
	FString nextMap;
	nextMap = FString("NextMap=");
	nextMap += IPAddress;

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Loading"), true, nextMap);
	//UGameplayStatics::OpenLevel(this, FName(*IPAddress));
}
