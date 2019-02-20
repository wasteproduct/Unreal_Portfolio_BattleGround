// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetCrosshair.h"

UWidgetCrosshair::UWidgetCrosshair(const FObjectInitializer&ObjectInitializer)
	:Super(ObjectInitializer), Crosshair(nullptr)
{

}

void UWidgetCrosshair::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidgetCrosshair::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
