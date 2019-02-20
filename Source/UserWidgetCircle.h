// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetCircle.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEACTOR_API UUserWidgetCircle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUserWidgetCircle(const FObjectInitializer&ObjectInitializer);
	
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry&MyGeometry, float InDeltaTime)override;

	virtual void NativePaint(FPaintContext&InContext)const override;

	void SetCircleColor(const FLinearColor&circleColor);
	void SetLocationRadius(const FVector2D&initialLocation, const float&initialRadius);

private:
	FVector2D centerLocation;
	float radius;
	FLinearColor color;
};
