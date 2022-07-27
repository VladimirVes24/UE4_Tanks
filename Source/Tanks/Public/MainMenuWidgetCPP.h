// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "URadioButtons.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidgetCPP.generated.h"

class UWidgetSwitcher;
class UButton;

UCLASS(Abstract)
class TANKS_API UMainMenuWidgetCPP : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UWidgetSwitcher* WidgetSwitcherMenu;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UButton* PlayButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* QuitButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UButton* OptionsButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* BackToMenuButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UURadioButtons* RadioButtons;

	

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnBackClicked();
	
};
