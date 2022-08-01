// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TanksHUD.generated.h"

UENUM()
enum class EWidgetID : uint8
{
	Wid_Default = 0x0,
	Wid_MainMenu = 0x1,
	Wid_GameOverMenu = 0x2,
	Wid_PauseMenu = 0x3,
	Wid_GameHud = 0x4,
	Wid_TankList = 0x5,
	Wid_SpawnList = 0x6
};

UCLASS()
class TANKS_API ATanksHUD : public AHUD
{
	GENERATED_BODY()
public:
	ATanksHUD();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UUserWidget* ShowWidget(const EWidgetID WidgetId, const int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable)
	void HideWidget();

	UFUNCTION(BlueprintPure)
	UUserWidget* GetCurrentWidget() const { return CurrentWidget; }

protected:
	UUserWidget* CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder = 0);

	UPROPERTY(EditAnywhere)
	TMap<EWidgetID, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
	UUserWidget* CurrentWidget;
	
	
};
