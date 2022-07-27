// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RadioButtonsWidgetStyle.h"
#include "Components/Widget.h"
#include "URadioButtons.generated.h"

/**
 * 
 */
class SRadioButtons;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioButtonChangedEvent, int32, NewSelectedButton);

UCLASS()
class TANKS_API UURadioButtons : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnRadioButtonChangedEvent OnRadioButtonChanged;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UPROPERTY(EditAnywhere, Category = Appearance, meta=(DisplayName=Style))
	FRadioButtonsStyle WidgetStyle;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RowsCount = 4;

private:

	TSharedPtr<SRadioButtons> RadioButtons;

	void OnRadioButtonsChangedFunc(int32 NewSelectedButton);

	int32 GetCount() const { return RowsCount; }
	
};
