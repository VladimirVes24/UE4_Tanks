// Fill out your copyright notice in the Description page of Project Settings.


#include "MyButtonStyle.h"
#include "Engine.h"
#include "Slate/SlateGameResources.h"
#include "Styling/ISlateStyle.h"


TSharedPtr<ISlateStyle> FMyButtonStyle::ButtonStylePtr;

TSharedPtr<ISlateStyle> FMyButtonStyle::CreateStyle()
{
	const FString ScopeToDirectory("/Game/UI");
	TSharedPtr<ISlateStyle> ButtonStyle = FSlateGameResources::New(FName("MyButtonStyle"), ScopeToDirectory, ScopeToDirectory);

	return ButtonStyle;
}

void FMyButtonStyle::Initialize()
{
	Shutdown();

	ButtonStylePtr = CreateStyle();
	FSlateStyleRegistry::RegisterSlateStyle(*ButtonStylePtr);
	
}

void FMyButtonStyle::Shutdown()
{
	if (ButtonStylePtr.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*ButtonStylePtr);
		ButtonStylePtr.Reset();
	}
}

const ISlateStyle& FMyButtonStyle::Get()
{
	if (!ButtonStylePtr.IsValid())
	{
		Initialize();
	}
	return *ButtonStylePtr;
}
