// Fill out your copyright notice in the Description page of Project Settings.


#include "URadioButtons.h"
#include "SRadioButtons.h"


void UURadioButtons::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	RadioButtons.Reset();
}

TSharedRef<SWidget> UURadioButtons::RebuildWidget()
{
	RadioButtons = SNew(SRadioButtons)
		.OnRadioButtonChanged_UObject(this, &ThisClass::OnRadioButtonsChangedFunc)
		.Count_UObject(this, &ThisClass::GetCount)
		.Style(&WidgetStyle)
		;
	return RadioButtons.ToSharedRef();
}

void UURadioButtons::OnRadioButtonsChangedFunc(int32 NewSelectedButton)
{
	OnRadioButtonChanged.Broadcast(NewSelectedButton);
}
