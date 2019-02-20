// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBattleRoyal.h"
#include "UObject/ConstructorHelpers.h"
#include "UserWidgetClientMain.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameModeBattleRoyal.h"
#include "UserWidgetResult.h"
#include "WidgetBlueprintLibrary.h"
#include "CharacterC130.h"
#include "PawnFlyingViewer.h"
#include "CharacterPlayerBase.h"
#include "UserWidgetWorldMap.h"

APlayerControllerBattleRoyal::APlayerControllerBattleRoyal()
	:clientMain(nullptr), jumpedOut(false), worldMap(nullptr), mapOpened(false), flyingViewer(nullptr), c130(nullptr),
	baseCharacter(nullptr)
{
	ConstructorHelpers::FClassFinder<UUserWidgetClientMain>clientMainClass(TEXT("/Game/GameModes/Battle/UMG/Blueprint_Widget_ClientMain"));

	if (clientMainClass.Succeeded())
	{
		subclassWidgetClientMain = clientMainClass.Class;
	}

	//

	ConstructorHelpers::FClassFinder<UUserWidgetResult>resultClass(TEXT("/Game/GameModes/Battle/UMG/Blueprint_Widget_Result"));

	if (resultClass.Succeeded())
	{
		subclassWidgetResult = resultClass.Class;
	}

	//

	ConstructorHelpers::FClassFinder<UUserWidgetWorldMap>worldMapClass(TEXT("/Game/WorldMap/Blueprint_Widget_WorldMap"));

	if (worldMapClass.Succeeded())
	{
		subclassWidgetWorldMap = worldMapClass.Class;
	}
}

void APlayerControllerBattleRoyal::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor *>outActors;

	UGameplayStatics::GetAllActorsWithTag(this, TEXT("C130"), outActors);

	if (outActors.Num() > 0)
	{
		c130 = Cast<ACharacterC130>(outActors[0]);

		if (IsValid(c130) == true)
		{

		}
		else
		{
			UKismetSystemLibrary::PrintString(this, FString("C130 NOT found."), true, true, FLinearColor::Red, 30.0f);
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, FString("C130 NOT found."), true, true, FLinearColor::Red, 30.0f);
	}

	//
	
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);

	bShowMouseCursor = false;

	//서버
	if (HasAuthority() == true)
	{

	}
	//클라이언트
	else
	{
		if (subclassWidgetClientMain)
		{
			clientMain = CreateWidget<UUserWidgetClientMain>(this, subclassWidgetClientMain);
		}

		if (clientMain)
		{
			clientMain->AddToViewport();
		}

		if (subclassWidgetWorldMap)
		{
			worldMap = CreateWidget<UUserWidgetWorldMap>(this, subclassWidgetWorldMap);
		}
	}
}

void APlayerControllerBattleRoyal::SetNotificationFromServer(const FName & newText)
{
	notificationFromServer = newText;
}

void APlayerControllerBattleRoyal::KillPlayer(void)
{
	if (UGameplayStatics::GetGameMode(this))
	{
		AGameModeBattleRoyal *currentGameMode = nullptr;
		currentGameMode = Cast<AGameModeBattleRoyal>(UGameplayStatics::GetGameMode(this));

		if (currentGameMode)
		{
			currentGameMode->KillPlayer(this);
		}
	}
}

FVector APlayerControllerBattleRoyal::GetPawnLocation(void)
{
	if (IsValid(GetPawn()) == true)
	{
		if (BaseCharacter() == true)
		{
			if (IsValid(baseCharacter) == true)
			{
				return baseCharacter->GetMeshLocation();
			}
		}
		if (InC130() == true)
		{
			if (IsValid(c130) == true)
			{
				return c130->GetMeshLocation();
			}
		}
	}
	return FVector();
}

bool APlayerControllerBattleRoyal::InC130(void)
{
	flyingViewer = Cast<APawnFlyingViewer>(GetPawn());

	if (IsValid(flyingViewer) == true)return true;
	return false;
}

FVector APlayerControllerBattleRoyal::GetPawnForwardVector(void)
{
	if (InC130() == true)
	{
		if (IsValid(c130) == true)
		{
			return c130->GetMeshForwardVector()*-1.0f;
		}
		return FVector();
	}
	if (BaseCharacter() == true)
	{
		if (IsValid(baseCharacter) == true)
		{
			return baseCharacter->GetMeshForwardVector();
		}
		return FVector();
	}
	return FVector();
}

bool APlayerControllerBattleRoyal::Client_OutToLobby_Validate()
{
	return true;
}

void APlayerControllerBattleRoyal::Client_OutToLobby_Implementation()
{
	UGameplayStatics::OpenLevel(this, TEXT("Lobby"));
}

bool APlayerControllerBattleRoyal::Server_BoardC130_Validate(ACharacterC130 * c130)
{
	return true;
}

void APlayerControllerBattleRoyal::Server_BoardC130_Implementation(ACharacterC130 * c130)
{
	GetWorld()->DestroyActor(GetPawn());

	UnPossess();

	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Owner = this;
	
	APawnFlyingViewer *aPawn = nullptr;
	aPawn = GetWorld()->SpawnActor<APawnFlyingViewer>(APawnFlyingViewer::StaticClass(), c130->GetActorTransform(), spawnParameters);

	if (aPawn)
	{
		Possess(aPawn);

		aPawn->AttachToActor(c130, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false));
	}
}

bool APlayerControllerBattleRoyal::Server_JumpOut_Validate()
{
	return true;
}

void APlayerControllerBattleRoyal::Server_JumpOut_Implementation()
{
	if (jumpedOut == true)return;

	jumpedOut = true;

	FVector location;
	location = GetPawn()->GetActorLocation();
	location.Z -= 1000.0f;

	FTransform transform;
	transform.SetLocation(location);

	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Owner = this;

	ACharacterPlayerBase *aPawn = nullptr;
	aPawn = GetWorld()->SpawnActor<ACharacterPlayerBase>(ACharacterPlayerBase::StaticClass(), transform, spawnParameters);

	GetWorld()->DestroyActor(GetPawn());

	if (aPawn)
	{
		Possess(aPawn);

		aPawn->SetFalling(true);
	}
}

bool APlayerControllerBattleRoyal::Client_PlayScreenEffectBlood_Validate(void)
{
	return true;
}

void APlayerControllerBattleRoyal::Client_PlayScreenEffectBlood_Implementation(void)
{
	if (IsValid(clientMain) == true)
	{
		if (clientMain->screenEffectBlood)
		{
			clientMain->PlayAnimation(clientMain->screenEffectBlood);
		}
	}
}

void APlayerControllerBattleRoyal::SetMainWidgetText()
{
	if (clientMain)
	{
		clientMain->SetText(notificationFromServer);
	}
}

void APlayerControllerBattleRoyal::WorldMapOnOff()
{
	mapOpened = !mapOpened;

	if (IsValid(worldMap) == true)
	{
		if (mapOpened == true)
		{
			worldMap->AddToViewport();
		}
		else
		{
			worldMap->RemoveFromParent();
		}
	}
}

bool APlayerControllerBattleRoyal::BaseCharacter(void)
{
	baseCharacter = Cast<ACharacterPlayerBase>(GetPawn());

	if (IsValid(baseCharacter) == true)return true;
	return false;
}

void APlayerControllerBattleRoyal::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("WorldMap", EInputEvent::IE_Released, this, &APlayerControllerBattleRoyal::WorldMapOnOff);
}

bool APlayerControllerBattleRoyal::Client_DisplayResult_Validate(bool winner, int32 rank)
{
	return true;
}

void APlayerControllerBattleRoyal::Client_DisplayResult_Implementation(bool winner, int32 rank)
{
	if (subclassWidgetResult)
	{
		UUserWidgetResult *widgetResult = nullptr;
		widgetResult = CreateWidget<UUserWidgetResult>(this, subclassWidgetResult);

		if (widgetResult)
		{
			widgetResult->SetResult(winner, FName(*FString::FromInt(rank)));

			widgetResult->AddToViewport();

			UWidgetBlueprintLibrary::SetInputMode_UIOnly(this, widgetResult);

			bShowMouseCursor = true;
		}
	}
}

void APlayerControllerBattleRoyal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerControllerBattleRoyal, notificationFromServer);
}
