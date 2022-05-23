// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthInGame.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API UHealthInGame : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);
protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* HealthBar;
	
};
