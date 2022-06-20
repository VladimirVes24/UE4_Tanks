// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "DamageTaker.h"
#include "Scorable.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//OnKill.AddUObject(this, &AProjectile::GetScoreForKill)
	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = Collision;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionObjectType(ECC_GameTraceChannel1);
}

//int AProjectile::GetScoreForKill(int Score)
//{
//	return Score;
//}

//void AProjectile::GetScoreForKill(AActor* Destroyer, AActor* DestroyedActor)
//{
//	if (EarnedScore != 0 && OnKill.IsBound()) OnKill.Broadcast(EarnedScore);
//}

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
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
    AActor* ProjectileOwner = GetOwner();
    AActor* OwnerOfProjectileOwner;
	if (ProjectileOwner != nullptr)
	{
		OwnerOfProjectileOwner = ProjectileOwner->GetOwner();
	}
	else
	{
		OwnerOfProjectileOwner = nullptr;
	}
    if(OtherActor != ProjectileOwner && OtherActor != OwnerOfProjectileOwner)
    {
		IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(OtherActor);
		if(DamageTakerActor)
		{
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.Instigator = ProjectileOwner;
			DamageData.DamageMaker = this;
			IScorable* ScorableTarget = Cast<IScorable>(OtherActor);
			if ( ScorableTarget )
			{
				EarnedScore = ScorableTarget->GrantScore();
			}
			DamageTakerActor->TakeDamage(DamageData);
			//const int ScoreForKill = 
			if (!OtherActor && OnKill.IsBound()) OnKill.Broadcast(EarnedScore);
    	
    }
    //
    	//if
    Destroy();
    }


	//GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
	//UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s "), *GetName(), *OtherActor->GetName());
	//OtherActor->Destroy();
	//Destroy();
}

void AProjectile::MoveTick()
{
	const auto Location = GetActorLocation();
	SetActorLocation(Location + GetActorForwardVector()*MoveRate*MoveSpeed, true);
}

// Called every frame


