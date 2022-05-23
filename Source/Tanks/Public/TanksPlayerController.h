// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "TankPawn.h"
#include "GameFramework/PlayerController.h"
#include "TanksPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API ATanksPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	void OnTryingToMoveForward(float InputPushScale);
	void OnTryingToMoveRight(float InputPushScale);
	void OnTryingToRotateRight(float InputPushScale);
	void OnTryingToChangeFireRate();
	void OnShoot();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	FVector GetMousePosition() const { return MouseWorldPosition; }

private:
	UPROPERTY()
	ATankPawn* PlayerTank;
	FVector MouseWorldPosition;
};
