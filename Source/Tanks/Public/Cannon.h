// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "MatineeCameraShake.h"
#include "Cannon.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireProjectile = 0 UMETA(DisplayName = "Projectile Cannon"),
	FireTrace = 1 UMETA(DisplayName = "Trace Cannon"),
	FireAuto = 2 UMETA(DisplayName = "Semi-auto Cannon"),
	FireMachineGun = 3 UMETA(DisplayName = "MachineGun")
};

UCLASS()
class TANKS_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	USphereComponent* CannonBase;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* CannonBaseMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* CannonMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	float FireRate = 1.0f;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	float TracePulsesPerSecond = 70.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	float TraceDamagePerSecond = 20.0f;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	float TraceThickness = 30.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	int BurstSize = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	float FireRange = 10000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	float FireDamage = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	ECannonType Type = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire params")
	TSubclassOf<AProjectile> ProjectileClass;
	
	bool Shoot(int ShootAmount);
	void ShootProjectile();
	void ShootBurst();
	//void ShootContinuously();
	void StartTrace();
	void ShootTrace();
	void StopTrace();
	void CycleFireMode();
	bool IsReadyToFire();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ShootEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UForceFeedbackEffect * ShootForceEffect;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> ShootShake;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float GetTimerValue() const;
	AProjectile* GetLastProjectilePointer() const;


private:
	void ResetShootState();
	FTimerHandle ReloadTimer, ShootRateTimer, ShootingTimer, TraceTimer;

	bool bReadyToShoot = true;
	int CallTracker = 0;
	UPROPERTY()
	AProjectile* LastProjectile = nullptr;
	
};


