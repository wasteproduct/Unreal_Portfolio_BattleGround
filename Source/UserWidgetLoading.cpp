// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidgetLoading.h"

UUserWidgetLoading::UUserWidgetLoading(const FObjectInitializer&ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UUserWidgetLoading::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUserWidgetLoading::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
