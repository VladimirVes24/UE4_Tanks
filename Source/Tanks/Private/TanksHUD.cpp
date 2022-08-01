// Fill out your copyright notice in the Description page of Project Settings.


#include "TanksHUD.h"

#include "Blueprint/UserWidget.h"

ATanksHUD::ATanksHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATanksHUD::BeginPlay()
{
	Super::BeginPlay();

	//ShowWidget(EWidgetID::Wid_TankList, 0);
}

void ATanksHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UUserWidget* ATanksHUD::ShowWidget(const EWidgetID WidgetId, const int32 ZOrder)
{
	HideWidget();
	TSubclassOf<UUserWidget>* ClassPtr = WidgetClasses.Find(WidgetId);
	if (ClassPtr && *ClassPtr)
	{
		CreateWidgetByClass(*ClassPtr, ZOrder);
	}

	return CurrentWidget;
}

void ATanksHUD::HideWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

UUserWidget* ATanksHUD::CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder)
{
	CurrentWidget = CreateWidget(GetOwningPlayerController(), WidgetClass);
	CurrentWidget->AddToViewport(ZOrder);
	return CurrentWidget;
}
