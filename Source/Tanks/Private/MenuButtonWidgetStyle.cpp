// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuButtonWidgetStyle.h"

FMenuButtonStyle::FMenuButtonStyle()
{
	ButtonStyle = FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button");
}

FMenuButtonStyle::~FMenuButtonStyle()
{
}

const FName FMenuButtonStyle::TypeName(TEXT("FMenuButtonStyle"));

const FMenuButtonStyle& FMenuButtonStyle::GetDefault()
{
	static FMenuButtonStyle Default;
	return Default;
}

void FMenuButtonStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

