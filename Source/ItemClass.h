// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Components/SkeletalMeshComponent.h"

#include "ItemClass.generated.h"

UENUM(BlueprintType)
enum class EItemEquipmentType :uint8
{
	Equipment_None,
	Equipment_Upper,
	Equipment_Lower,
	Equipment_Head,
	Equipment_Feet,
	Equipment_Hands
};

USTRUCT(BlueprintType)
struct FItemClass :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		int itemClassID;

	UPROPERTY(EditAnywhere)
		FName itemName;

	UPROPERTY(EditAnywhere)
		EItemEquipmentType itemEquipmentType;

	UPROPERTY(EditAnywhere)
		USkeletalMesh *skeletalMesh;

	UPROPERTY(EditAnywhere)
		UStaticMesh *staticMesh;

	FItemClass()
	{
		itemClassID = 0;
		itemName = FName("");
		itemEquipmentType = EItemEquipmentType::Equipment_None;
		skeletalMesh = nullptr;
		staticMesh = nullptr;
	}
};

/**
 * 
 */
//class PRACTICEACTOR_API ItemClass
//{
//public:
//	ItemClass();
//	~ItemClass();
//};
