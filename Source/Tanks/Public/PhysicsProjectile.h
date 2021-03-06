// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PhysicsProjectile.generated.h"

class UPhysicsComponent;
/**
 * 
 */
UCLASS()
class TANKS_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()

	protected:
		UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UPhysicsComponent* PhysicsComponent;
	
		UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent * TrailEffect;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float MoveAccuracy = 10;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationMaxTime = 50;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationTimeStep = 1;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		float TrajectorySimulationSpeed = 20;

		//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		//float ExplodeRadius = 500;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
		bool ShowTrajectory = false;
	
		UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		FVector MoveVector;
	
		UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		TArray<FVector> CurrentTrajectory;
	
		UPROPERTY(BlueprintReadWrite, Category = "Movement params")
		int32 TrajectoryPointIndex;
    public:
		APhysicsProjectile();
		virtual void Start() override;
	protected:
		virtual void MoveTick() override;
		virtual void Explode() override;

};
