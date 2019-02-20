// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetCircle.h"
#include "WidgetBlueprintLibrary.h"
#include "WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UUserWidgetCircle::UUserWidgetCircle(const FObjectInitializer&ObjectInitializer)
	:Super(ObjectInitializer)
{
	centerLocation = FVector2D(0.0f, 0.0f);
	radius = 0.0f;
}

void UUserWidgetCircle::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUserWidgetCircle::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUserWidgetCircle::NativePaint(FPaintContext & InContext) const
{
	Super::NativePaint(InContext);

	for (int32 i = 0; i < 36; ++i)
	{
		FVector a;
		FVector b;

		a = UKismetMathLibrary::RotateAngleAxis(FVector(0.0f, radius, 0.0f), 10.0f*(float)i, FVector(0.0f, 0.0f, 1.0f));
		b = UKismetMathLibrary::RotateAngleAxis(FVector(0.0f, radius, 0.0f), 10.0f*(float)(i + 1), FVector(0.0f, 0.0f, 1.0f));

		FVector2D positionA;
		FVector2D positionB;
		
		positionA.Set(a.X, a.Y);
		positionB.Set(b.X, b.Y);

		positionA += centerLocation;
		positionB += centerLocation;

		UWidgetBlueprintLibrary::DrawLine(InContext, positionA, positionB, color);
	}
}

void UUserWidgetCircle::SetCircleColor(const FLinearColor & circleColor)
{
	color = circleColor;
}

void UUserWidgetCircle::SetLocationRadius(const FVector2D & initialLocation, const float & initialRadius)
{
	centerLocation = initialLocation;
	radius = initialRadius;
}
