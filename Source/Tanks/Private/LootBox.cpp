// Fill out your copyright notice in the Description page of Project Settings.


#include "LootBox.h"

#include "TankPawn.h"

// Sets default values
ALootBox::ALootBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent * SceneCmp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneCmp;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->OnComponentBeginOverlap.AddDynamic(this, &ALootBox::OnMeshOverlapBegin);
    Mesh->SetCollisionProfileName(FName("OverlapAll"));
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetGenerateOverlapEvents(true);

}

void ALootBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* PlayerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (OtherActor == PlayerPawn)
	{
		if (bContainsMachineGun && ContainedMachineGunClass) PlayerPawn->SetupMachineGun(ContainedMachineGunClass);
		if (bContainsCannon && ContainedCannonClass) PlayerPawn->SetupCannon(ContainedMachineGunClass);
		if (bContainsAmmo)
		{
			const int NewAmmoAmount = PlayerPawn->AmmoLeft + ContainedAmmo;
			if (NewAmmoAmount >= PlayerPawn->AmmoStockSize) PlayerPawn->AmmoLeft = PlayerPawn->AmmoStockSize;
			else PlayerPawn->AmmoLeft += ContainedAmmo;
		}
		Destroy();
	}
}

