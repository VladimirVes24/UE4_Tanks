// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "HealthInGame.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "TankPawn.generated.h"

UCLASS(Blueprintable)
class TANKS_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxComponent2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxComponent3;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	//UBoxComponent* BoxComponent4;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	//UBoxComponent* BoxComponent5;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TankBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TankTurret;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* ArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<ACannon> CannonType;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<UHealthInGame> HealthWidget;
	
	UPROPERTY()
	class ACannon* Cannon;

	//UPROPERTY()
	//UHealthInGame* HealthWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float MovementSpeed = 800;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float MovementAcceleration = 0.02;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float RotationSpeed = 50;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float RotationAcceleration = 0.05;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float RotationSpeedTurret = 50;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float RotationAccelerationTurret = 0.02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float HealthPointsMax = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float HealthPointsCurrent = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float ReloadRate = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int ClipSize = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int AmmoStockSize = 12;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int AmmoLeft = 0;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int ClipContains = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Fire params")
	float FireRateTimerValue = 0;
	
	void SetMoveForwardPushScale(float Scale);
	void SetMoveRightPushScale(float Scale);
	void SetRotateRightScale(float Scale);

	int GetClipContains();

	void Shoot();
	void ChangeFireMode();
	void Reload(int ReloadAmount);
	void Flip();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

private:
	float TargetForwardPushScale = 0;
	float CurrentForwardMoveScale = 0;
	
	float RightPushScale = 0;
	
	float TargetRotationScale = 0;
	float CurrentRotationScale = 0;

	float TargetRotationScaleTurret = 0;
	float CurrentRotationScaleTurret = 0;
	
	FTimerHandle TimerHandle;
	bool bReloaded = true;
	bool bFlipped = false;
	void ResetReloadState();
	
	
    void MoveTank(float DeltaTime);
	void RotateTank(float DeltaTime);
	void RotateCannon(float Deltatime);
	

	UPROPERTY()
	class ATanksPlayerController* TankController;
	
};

