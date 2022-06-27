// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKS_API AProjectile : public AActor
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(UHealthComponent, FOnKill, int)
	
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	FOnKill OnKill;
	//UFUNCTION()
	//void GetScoreForKill(AActor OnDestroyed, AActor* DestroyedActor);
	virtual void Start();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    float PushForce = 1000;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	USphereComponent* Collision;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	float MoveSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	float MoveRate = 0.005f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	bool IsExplosionEnabled = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ExplodeRadius = 500;
	
private:
	FTimerHandle MovementTimerHandle;
	int EarnedScore = 0;

	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult);

	UFUNCTION()
	virtual void MoveTick();
	UFUNCTION()
	virtual void Explode();

};
