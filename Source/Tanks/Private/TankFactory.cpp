// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"

#include "MapLoader.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATankFactory::ATankFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent * sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComp;
	
	FactoryBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FactoryBase"));
	FactoryBase->SetupAttachment(sceneComp);
	FactoryMount = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Factory Mount"));
	FactoryMount->SetupAttachment(FactoryBase);
	FactoryArm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Factory Arm"));
	FactoryArm->SetupAttachment(FactoryMount);
	BrokenFactoryArm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destroyed Factory Arm"));
	BrokenFactoryArm->SetupAttachment(FactoryMount);
	
	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->AttachToComponent(sceneComp,FAttachmentTransformRules::KeepRelativeTransform);
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	CollisionBox->SetupAttachment(sceneComp);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATankFactory::DamageTaken);

	DeathExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>("Explosion of death");
	DeathExplosionEffect->SetupAttachment(sceneComp);
	
	TankSpawnEffect = CreateDefaultSubobject<UParticleSystemComponent>("Spawn Effect");
	TankSpawnEffect->SetupAttachment(sceneComp);
}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	DeathExplosionEffect->Deactivate();
	BrokenFactoryArm->SetHiddenInGame(true);
	if(LinkedMapLoader)
    LinkedMapLoader->SetIsActivated(false);
	
    GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
}

void ATankFactory::SpawnNewTank()
{
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(),TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn * newTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass,	spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	newTank->SetPatrollingPoints(TankWayPoints);

	UGameplayStatics::FinishSpawningActor(newTank, spawnTransform);
	TankSpawnEffect->Activate();
}

void ATankFactory::Die()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	DeathExplosionEffect->ActivateSystem();
	BrokenFactoryArm->SetHiddenInGame(false);
	FactoryArm->SetHiddenInGame(true);
	if(LinkedMapLoader)
		LinkedMapLoader->SetIsActivated(true);
}

void ATankFactory::DamageTaken(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

// Called every frame
void ATankFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

