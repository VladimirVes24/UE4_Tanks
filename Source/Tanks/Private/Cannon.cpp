// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"

#include "DrawDebugHelpers.h"


// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CannonBase = CreateDefaultSubobject<USphereComponent>("CannonBase");
	RootComponent = CannonBase;

	CannonBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("CannonBaseMesh");
	CannonBaseMesh->SetupAttachment(RootComponent);
	
	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	CannonMesh->SetupAttachment(CannonBaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>("ProjectileSpawnPoint");
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

bool ACannon::Shoot(int ShootAmount)
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
		ShootTrace();
		break;
	case ECannonType::FireAuto:
		CallTracker = ShootAmount;
		GetWorldTimerManager().SetTimer(ShootRateTimer, this, &ACannon::ShootBurst, FireRate/BurstSize, true);
		break;
	case ECannonType::FireMachineGun:
		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
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
	if (CallTracker <=0 ) GetWorldTimerManager().ClearTimer(ShootRateTimer);
}

void ACannon::ShootTrace()
{
	FHitResult HitResult;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	FVector StartPoint = ProjectileSpawnPoint->GetComponentLocation();
	FVector EndPoint = ProjectileSpawnPoint->GetForwardVector() * FireRange + StartPoint;
	if(GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, TraceParams))
	{
		DrawDebugLine(GetWorld(), StartPoint, HitResult.Location, FColor::Red, false, 0.5f, 1, 15);

		if (HitResult.Actor.Get())
		{
			HitResult.Actor.Get()->Destroy();
		}
	}
	else DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 0.5f, 1, 50);
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

