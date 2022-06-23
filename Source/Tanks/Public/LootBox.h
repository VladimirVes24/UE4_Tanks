// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LootBox.generated.h"

UCLASS()
class TANKS_API ALootBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootBox();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	bool bContainsMachineGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	bool bContainsCannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	bool bContainsAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TSubclassOf<ACannon> ContainedCannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TSubclassOf<ACannon> ContainedMachineGunClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	int ContainedAmmo = 50;
	
	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult);
};
