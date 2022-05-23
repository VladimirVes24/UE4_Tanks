// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidgetCPP.h"
#include  "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UMainMenuWidgetCPP::NativeConstruct()
{
	Super::NativeConstruct();
	if(QuitButton)
	{
		QuitButton->OnPressed.AddDynamic(this, &ThisClass::OnQuitClicked);
	}
	if(OptionsButton)
	{
		OptionsButton->OnPressed.AddDynamic(this, &ThisClass::OnOptionsClicked);
	}
	if(BackToMenuButton)
	{
		BackToMenuButton->OnPressed.AddDynamic(this, &ThisClass::UMainMenuWidgetCPP::OnBackClicked);
	}
}
void UMainMenuWidgetCPP::NativeDestruct()
{
	if (PlayButton)
	{
		PlayButton->OnPressed.RemoveAll(this);
	}
	if(QuitButton)
	{
		QuitButton->OnPressed.RemoveAll(this);
	}

	if(BackToMenuButton)
	{
		BackToMenuButton->OnPressed.RemoveAll(this);
	}
	Super::NativeDestruct();
}
void UMainMenuWidgetCPP::OnQuitClicked()
{
	GEngine->Exec(GetWorld(), TEXT("Exit"));
}
void UMainMenuWidgetCPP::OnOptionsClicked()
{
	WidgetSwitcherMenu->SetActiveWidgetIndex(1);
}
void UMainMenuWidgetCPP::OnBackClicked()
{
	WidgetSwitcherMenu->SetActiveWidgetIndex(0);
}


