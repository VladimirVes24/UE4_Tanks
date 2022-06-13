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
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
private:
	FTimerHandle MovementTimerHandle;

	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult);

	UFUNCTION()
	void MoveTick();

};
