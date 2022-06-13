// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = Collision;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionObjectType(ECC_GameTraceChannel1);
}
// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::MoveTick, MoveRate, true);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s "), *GetName(), *OtherActor->GetName());
	OtherActor->Destroy();
	Destroy();
}

void AProjectile::MoveTick()
{
	const auto Location = GetActorLocation();
	SetActorLocation(Location + GetActorForwardVector()*MoveRate*MoveSpeed, true);
}

// Called every frame


