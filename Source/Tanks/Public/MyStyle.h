// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct TANKS_API FMyStyle
{
public:
	static void Initialize();
	static void Shutdown();
	static const ISlateStyle& Get();
private:
	static TSharedPtr<ISlateStyle> StylePtr;
};
