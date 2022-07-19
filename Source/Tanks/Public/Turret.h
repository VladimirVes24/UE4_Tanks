// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Scorable.h"
#include "Turret.generated.h"

UCLASS()
class TANKS_API ATurret : public AActor, public IDamageTaker, public IScorable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();
	
protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BodyCollisionBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* TowerCollisionBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* CannonCollisionBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TowerMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonSetupPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<ACannon> CannonType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* DeathExplosionEffect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealthState;

	UPROPERTY()
	ACannon* Cannon;

	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRate = 0.005f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accuracy = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Score")
	float GrantedScoreOnDeath = 10;
	const FString BodyMeshPath ="StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TowerMeshPath ="StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";
	

	bool IsPlayerSeen();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
    virtual void TakeDamage(FDamageData DamageData) override;
	virtual float GrantScore() override;
	UFUNCTION()
    void Die();
    UFUNCTION()
    void DamageTaken(float DamageValue);
	FVector GetEyesPosition();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	void Targeting();
    void RotateToPlayer();
    bool IsPlayerInRange();
    bool IsAimingAtTarget();
    void Fire();

	
};
