// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"


// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	
	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	CannonMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>("ProjectileSpawnPoint");
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

bool ACannon::Shoot(int LeftInClip)
{
	
	if (!bReadyToShoot)
		return false;
	switch (Type)
	{
	case ECannonType::FireProjectile:
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString(TEXT("Bum")));
		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		break;
	case ECannonType::FireTrace:
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString(TEXT("Bum")));
		break;
	case ECannonType::FireAuto:
		CallTracker = LeftInClip;
		GetWorldTimerManager().SetTimer(TimerHandle2, this, &ACannon::ShootBurst, FireRate/BurstSize, true);
		break;
	}
	bReadyToShoot = false;
	
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, FTimerDelegate::CreateUObject(this,&ACannon::ResetShootState), 1/FireRate, false);
	return true;
}


void ACannon::ShootBurst()
{
	--CallTracker;
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, FString(TEXT("Bum")));
	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	if (CallTracker <=0 ) GetWorldTimerManager().ClearTimer(TimerHandle2);
}

void ACannon::ResetShootState()
{
	bReadyToShoot=true;
}

void ACannon::CycleFireMode()
{
	switch (Type) {
		case ECannonType::FireProjectile:
			Type = ECannonType::FireTrace;
		GEngine->AddOnScreenDebugMessage(555, 3, FColor::White, FString::Printf(TEXT("Fire mode set to Trace")));
			break;

		case ECannonType::FireTrace:
			Type = ECannonType::FireAuto;
		GEngine->AddOnScreenDebugMessage(555, 3, FColor::White, FString::Printf(TEXT("Fire mode set to Semi-auto")));
			break;

		case ECannonType::FireAuto:
			Type = ECannonType::FireProjectile;
		GEngine->AddOnScreenDebugMessage(555, 3, FColor::White, FString::Printf(TEXT("Fire mode set to Single")));
			break;
	}

}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//GEngine->AddOnScreenDebugMessage(12354, -1, FColor::Blue, FString::Printf(TEXT("%f"), GetWorld()->GetTimerManager().GetTimerElapsed(TimerHandle)));
}

float ACannon::GetTimerValue() const
{
	return GetWorld()->GetTimerManager().GetTimerElapsed(ReloadTimer);
}

