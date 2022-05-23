// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthInGame.h"

void UHealthInGame::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	HealthBar->SetPercent(CurrentHealth/MaxHealth);
}
