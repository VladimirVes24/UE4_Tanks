// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnRadioButtonChanged, int32 /*NewSelectedIndex*/)

class TANKS_API SRadioButtons : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRadioButtons)
	{}
	SLATE_ATTRIBUTE(int32, Count);
	
	SLATE_EVENT(FOnRadioButtonChanged, OnRadioButtonChanged);
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	FOnRadioButtonChanged OnRadioButtonChanged;
private:
	TSharedRef<SWidget> CreateCheckbox(int32 InIndex, FString InText);

	int32 CurrentIndex = 0;

	ECheckBoxState IsChecked(int32 InIndex) const;

	void OnCheckboxStateChanged(ECheckBoxState NewState, int32 InIndex);

	TSharedPtr<SVerticalBox> RadioBox;
};
