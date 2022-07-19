// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
    HealthComponent->OnDie.AddUObject(this, &ATurret::Die);
	//HealthComponent->OnDie.AddUObject(this, &ATurret::GrantScore);
    HealthComponent->OnDamaged.AddUObject(this, &ATurret::DamageTaken);

	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret body"));
    RootComponent = BodyMesh;

	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret turret"));
    TowerMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
    CannonSetupPoint->AttachToComponent(TowerMesh, FAttachmentTransformRules::KeepRelativeTransform);

	UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TowerMeshPath);
    if(TurretMeshTemp) TowerMesh->SetStaticMesh(TurretMeshTemp);

	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
    if(BodyMeshTemp) BodyMesh->SetStaticMesh(BodyMeshTemp);

	BodyCollisionBox = CreateDefaultSubobject<UBoxComponent>("Body Collision");
	BodyCollisionBox->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	TowerCollisionBox = CreateDefaultSubobject<UBoxComponent>("Tower Collision");
	TowerCollisionBox->AttachToComponent(TowerMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonCollisionBox = CreateDefaultSubobject<UBoxComponent>("Cannon Collision");
	CannonCollisionBox->AttachToComponent(TowerMesh, FAttachmentTransformRules::KeepRelativeTransform);

	DeathExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>("Explosion on death");
	DeathExplosionEffect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealthState = 1;
	FActorSpawnParameters Params;
    Params.Owner = this;
    Cannon = GetWorld()->SpawnActor<ACannon>(CannonType, Params);
    Cannon->AttachToComponent(CannonSetupPoint,
    FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this,
	&ATurret::Targeting, TargetingRate, true, TargetingRate);

}

void ATurret::Destroyed()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathExplosionEffect->Template, GetActorLocation(), GetActorRotation(),FVector(3),true,EPSCPoolMethod::None, true);
	if (Cannon) Cannon->Destroy();
}

void ATurret::Targeting()
{
	if(IsPlayerInRange())
    {
    RotateToPlayer();
		if(IsAimingAtTarget() && Cannon && Cannon->IsReadyToFire())
		{
			Fire();
		}
    }
   

}

void ATurret::RotateToPlayer()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
    FRotator CurrentRotation = TowerMesh->GetComponentRotation();
    TargetRotation.Pitch = CurrentRotation.Pitch;
    TargetRotation.Roll = CurrentRotation.Roll;
    TowerMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TargetingSpeed));

}

bool ATurret::IsPlayerInRange()
{
	return (FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange && IsPlayerSeen());
}

bool ATurret::IsAimingAtTarget()
{
	const FVector TargetingDirection = TowerMesh->GetForwardVector();
    FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
    DirectionToPlayer.Normalize();
    const float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDirection, DirectionToPlayer)));
    return AimAngle <= Accuracy;
}

void ATurret::Fire()
{
	if (Cannon)
	{
			int ShootAmount = 1;
			if (Cannon->Type == ECannonType::FireAuto) ShootAmount = Cannon->BurstSize;
			Cannon->Shoot(ShootAmount);
	}
		
}

bool ATurret::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = GetEyesPosition();
	FHitResult hitResult;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(this);
	traceParams.bReturnPhysicalMaterial = false;
	if(GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos,ECC_Visibility, traceParams))
	{
		if(hitResult.Actor.Get())
		{
			DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Cyan,
			false, 0.5f, 0, 10);
			return hitResult.Actor.Get() == PlayerPawn;
		}
	}
	//DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Cyan, false, 0.5f, 0, 10);
	return false;
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurret::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

float ATurret::GrantScore()
{
	//HealthComponent->OnDie.Broadcast();
	return GrantedScoreOnDeath;
}

void ATurret::Die()
{
	Destroy();
}

void ATurret::DamageTaken(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s took damage:%f  Current health:%f"), *GetName(),DamageValue, HealthComponent->GetHealth());
	CurrentHealthState = HealthComponent->GetHealthState();
}

FVector ATurret::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}

