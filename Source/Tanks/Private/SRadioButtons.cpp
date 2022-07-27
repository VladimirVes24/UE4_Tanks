// Fill out your copyright notice in the Description page of Project Settings.


#include "SRadioButtons.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRadioButtons::Construct(const FArguments& InArgs)
{
	OnRadioButtonChanged = InArgs._OnRadioButtonChanged;
	CheckBoxStyle = &InArgs._Style->CheckBoxStyle;
	TextStyle = &InArgs._Style->TextStyle;
	TAttribute<int32> Count = InArgs._Count;
	

	Count.Get();
	
	ChildSlot
	[
		SAssignNew(RadioBox, SVerticalBox)
	];
	
	if (RadioBox->NumSlots() != Count.Get())
	{
		RadioBox->ClearChildren();
		for (int32 i = 0; i < Count.Get(); ++i)
		{
			RadioBox->AddSlot()
			[
				CreateCheckbox(i, TEXT("Option  ") + FString::FromInt(i))
			];
		}
	}
	
}


TSharedRef<SWidget> SRadioButtons::CreateCheckbox(int32 InIndex, FString InText)
{
	return SNew(SCheckBox)
		.IsChecked_Raw(this, &SRadioButtons::IsChecked, InIndex)
		.OnCheckStateChanged_Raw(this, &SRadioButtons::OnCheckboxStateChanged, InIndex)
		.Style(CheckBoxStyle)
		[
			SNew(STextBlock)
			.Text(FText::FromString(InText))
			.TextStyle(TextStyle)
		];
	
}

ECheckBoxState SRadioButtons::IsChecked(int32 InIndex) const
{
	return InIndex == CurrentIndex ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SRadioButtons::OnCheckboxStateChanged(ECheckBoxState NewState, int32 InIndex)
{
	if (NewState == ECheckBoxState::Checked)
	{
		CurrentIndex = InIndex;
		OnRadioButtonChanged.ExecuteIfBound(CurrentIndex);
	}
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
