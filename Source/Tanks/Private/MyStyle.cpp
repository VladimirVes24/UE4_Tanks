// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStyle.h"
#include "Engine.h"
#include "Slate/SlateGameResources.h"
#include "Styling/ISlateStyle.h"


TSharedPtr<ISlateStyle> FMyStyle::StylePtr;

TSharedPtr<ISlateStyle> CreateStyle()
{
	const FString ScopeToDirectory("/Game/UI");
	TSharedPtr<ISlateStyle> Style = FSlateGameResources::New(FName("MyStyle"), ScopeToDirectory, ScopeToDirectory);

	return Style;
}

void FMyStyle::Initialize()
{
	Shutdown();

	StylePtr = CreateStyle();
	FSlateStyleRegistry::RegisterSlateStyle(*StylePtr);
	
}

void FMyStyle::Shutdown()
{
	if (StylePtr.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StylePtr);
		StylePtr.Reset();
	}
}

const ISlateStyle& FMyStyle::Get()
{
	if (!StylePtr.IsValid())
	{
		Initialize();
	}
	return *StylePtr;
}
