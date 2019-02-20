// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ClassElectricFieldPhase.generated.h"

USTRUCT(BlueprintType)
struct FElectricFieldPhase :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		int32 phase;

	UPROPERTY(EditAnywhere)
		FVector currentCircleLocation;

	UPROPERTY(EditAnywhere)
		float currentCircleRadius;

	UPROPERTY(EditAnywhere)
		FVector nextCircleLocation;

	UPROPERTY(EditAnywhere)
		float nextCircleRadius;

	UPROPERTY(EditAnywhere)
		int32 waitingCount;

	UPROPERTY(EditAnywhere)
		int32 nextCircleExposureCount;

	UPROPERTY(EditAnywhere)
		int32 shrinkingCount;

	UPROPERTY(EditAnywhere)
		bool randomPhase;

	UPROPERTY(EditAnywhere)
		float minimumDistanceToNextCircle;

	UPROPERTY(EditAnywhere)
		float maximumDistanceToNextCircle;

	FElectricFieldPhase()
	{
		phase = 0;
		currentCircleLocation = FVector(0.0f, 0.0f, 0.0f);
		currentCircleRadius = 0.0f;
		nextCircleLocation = FVector(0.0f, 0.0f, 0.0f);
		nextCircleRadius = 0.0f;
		waitingCount = 0;
		nextCircleExposureCount = 0;
		shrinkingCount = 0;
	}
};

/**
 * 
 */
//class PRACTICEACTOR_API ClassElectricFieldPhase
//{
//public:
//	ClassElectricFieldPhase();
//	~ClassElectricFieldPhase();
//};
