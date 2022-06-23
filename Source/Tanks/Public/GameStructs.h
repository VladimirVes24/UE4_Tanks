// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameStructs.generated.h"

USTRUCT()
struct FDamageData
{
GENERATED_BODY()
UPROPERTY()
float DamageValue;
UPROPERTY()
AActor * Instigator;
UPROPERTY()
AActor * DamageMaker;
};

USTRUCT()
struct FScoreData
{
	GENERATED_BODY()
	UPROPERTY()
	float ScoreValue;
	UPROPERTY()
	AActor * Instigator;
	UPROPERTY()
	AActor * DamageMaker;
};

