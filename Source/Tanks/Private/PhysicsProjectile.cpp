// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"

#include "DamageTaker.h"
#include "DrawDebugHelpers.h"
#include "GameStructs.h"
#include "PhysicsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));
	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail effect"));
	TrailEffect->SetupAttachment(RootComponent);
}

void APhysicsProjectile::Start()
{
	MoveVector = GetActorForwardVector() * TrajectorySimulationSpeed;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(),MoveVector, TrajectorySimulationMaxTime, TrajectorySimulationTimeStep, 0);
	if(ShowTrajectory)
	{
		for(FVector position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), position, 5, 8, FColor::Purple, true, 1, 0, 2);
		}
	}
	TrajectoryPointIndex = 0;
	Super::Start();
}

void APhysicsProjectile::MoveTick()
{
	FVector currentMoveVector = CurrentTrajectory[TrajectoryPointIndex] - GetActorLocation();
	currentMoveVector.Normalize();
	FVector newLocation = GetActorLocation() + currentMoveVector * MoveSpeed * MoveRate;
	SetActorLocation(newLocation);
	if(FVector::Distance(newLocation, CurrentTrajectory[TrajectoryPointIndex]) <= MoveAccuracy)
	{
		TrajectoryPointIndex++;
		if(TrajectoryPointIndex >= CurrentTrajectory.Num())
		{
			Explode();
			Destroy();
		}
		else
		{
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),CurrentTrajectory[TrajectoryPointIndex]);
			SetActorRotation(newRotation);
		}
	}
}

void APhysicsProjectile::Explode()
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + FVector(0.1f);
	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.TraceTag = "Explode Trace";
	TArray<FHitResult> AttackHit;
	FQuat Rotation = FQuat::Identity;
	
	bool sweepResult = GetWorld()->SweepMultiByChannel
	(
	AttackHit,
	startPos,
	endPos,
	Rotation,
	ECC_Visibility,
	Shape,
	params
	);
	
	GetWorld()->DebugDrawTraceTag = "Explode Trace";
	if(sweepResult)
	{
		for(FHitResult hitResult : AttackHit)
		{
			AActor* otherActor = hitResult.GetActor();
			if(!otherActor)
				continue;
			IDamageTaker * damageTakerActor = Cast<IDamageTaker>(otherActor);
			if(damageTakerActor)
			{
				FDamageData damageData;
				damageData.DamageValue = Damage;
				damageData.Instigator = GetOwner();
				damageData.DamageMaker = this;
				damageTakerActor->TakeDamage(damageData);
			}
			UPrimitiveComponent* mesh =	Cast<UPrimitiveComponent>(otherActor->GetRootComponent());
			if(mesh)
			{
				if(mesh->IsSimulatingPhysics())
				{
					FVector forceVector = otherActor->GetActorLocation() - GetActorLocation();
					forceVector.Normalize();
					mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
				}
			}
			
		}
	}
}

