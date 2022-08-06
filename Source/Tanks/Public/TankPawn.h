// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "MatineeCameraShake.h"	
#include "HealthComponent.h"
#include "DamageTaker.h"
#include "Engine/TargetPoint.h"
#include "Tanks/Inventory/InventoryComponent.h"
#include "Tanks/Inventory/InventoryManagerComponent.h"
#include "TankPawn.generated.h"

UCLASS(Blueprintable)
class TANKS_API ATankPawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent * HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* TankBodyCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* LeftTrackCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* RightTrackCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* TankTowerCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* CannonCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TankBodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TankTowerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* ArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonPosition;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* MachineGunPosition;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<ACannon> CannonType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<ACannon> MachineGunType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> ShootShake;

	UPROPERTY(EditDefaultsOnly)
	UInventoryComponent* LocalInventory;
	
	UPROPERTY(EditDefaultsOnly)
	UInventoryManagerComponent* InventoryManager;
	
	UPROPERTY()
	ACannon* Cannon;

	UPROPERTY()
	ACannon* MachineGun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float MovementSpeed = 800;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float MovementAcceleration = 0.02;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float RotationSpeed = 50;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float RotationAcceleration = 0.05;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float RotationSpeedTurret = 0.1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float RotationAccelerationTurret = 0.01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement | Speed")
	float CannonTiltMaxDegree = 30;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealthState;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Fire params")
	float FireRateTimerValue = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* DeathExplosionEffect;

	
	
	void SetMoveForwardPushScale(float Scale);
	void SetMoveRightPushScale(float Scale);
	void SetRotateRightScale(float Scale);
	void SetRotateTowerRightScale(float Scale);

	int GetClipContains() const;


	void Shoot();
	void OnShootStop();
	void FireCannon();
	void FireSpecial();
	void FireMachineGun();
	void ChangeFireMode() const;
	void Reload(int ReloadAmount);
	void Flip();
	void SwitchWeapon();
	void SetupCannon(TSubclassOf<ACannon> DesiredCannonType);
	void SetupMachineGun(TSubclassOf<ACannon> DesiredMachineGunType);
	UFUNCTION()
	virtual void TakeDamage(FDamageData DamageData) override;
	UFUNCTION()
	void IncreaseTotalScore(int InputScore);
	void OpenInventory();

	
	UFUNCTION()
	float GetMovementAccuracy() { return MovementAccuracy; }
	FVector GetEyesPosition();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | MoveParams | Patrolling", meta = (MakeEditWidget = true))
	TArray<ATargetPoint*> PatrollingPoints;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
    void Die();
	UFUNCTION()
	void ShakeCamera();
    UFUNCTION()
    void DamageTaken(float DamageValue);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | MoveParams | Patrolling")
	float MovementAccuracy = 300.0f;
	



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RotateCannon(FVector TargetPosition);

	UFUNCTION()
	FVector GetTurretForwardVector();
	UFUNCTION()
	TArray<FVector> GetPatrollingPoints();
	void SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints);
	

	


private:
	float TargetForwardPushScale = 0;
	float CurrentForwardMoveScale = 0;
	
	float RightPushScale = 0;
	
	float TargetRotationScale = 0;
	float CurrentRotationScale = 0;

	float TargetRotationScaleTurret = 0;
	float CurrentRotationScaleTurret = 0;
	float RotationMarginDegrees = 1;
	
	FTimerHandle TimerHandle;
	FTimerHandle MachineGunTimerHandle;
	bool bReloaded = true;
	bool bFlipped = false;
	bool bSwitchedToSecondary = false;
	void ResetReloadState(int ReloadAmount);
	
	
    void MoveTank(float DeltaTime);
	void RotateTank(float DeltaTime);
	
	void RotateCannonViaGamepad(float DeltaTime);
	void TiltCannon()const;
	


	UPROPERTY()
	class ATanksPlayerController* TankController;
	
};


