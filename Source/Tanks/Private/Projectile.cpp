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

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::MoveTick, MoveRate, true);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
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
    	UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
        if(mesh)
        {
			if(mesh->IsSimulatingPhysics())
			{
				 FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
				 forceVector.Normalize();
				 mesh->AddImpulse(forceVector* PushForce, NAME_None,false);
			}
        }
    	if (IsExplosionEnabled) Explode();
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

void AProjectile::Explode()
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + FVector(0.1f);
	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.TraceTag = "Explode Trace";
	TArray<FHitResult> AttackHit;
	FQuat Rotation = FQuat::Identity;
	
	bool sweepResult = GetWorld()->SweepMultiByChannel
	(
	AttackHit,
	startPos,
	endPos,
	Rotation,
	ECC_Visibility,
	Shape,
	params
	);
	
	GetWorld()->DebugDrawTraceTag = "Explode Trace";
	if(sweepResult)
	{
		for(FHitResult hitResult : AttackHit)
		{
			AActor* otherActor = hitResult.GetActor();
			if(!otherActor)
				continue;
			IDamageTaker * damageTakerActor = Cast<IDamageTaker>(otherActor);
			if(damageTakerActor)
			{
				FDamageData damageData;
				damageData.DamageValue = Damage;
				damageData.Instigator = GetOwner();
				damageData.DamageMaker = this;
				damageTakerActor->TakeDamage(damageData);
			}
			else
			{
				UPrimitiveComponent* mesh =	Cast<UPrimitiveComponent>(otherActor->GetRootComponent());
				if(mesh)
				{
					if(mesh->IsSimulatingPhysics())
					{
						FVector forceVector = otherActor->GetActorLocation() -
						GetActorLocation();
						forceVector.Normalize();
						mesh->AddForce(forceVector * PushForce, NAME_None, true);
					}
				}
			}
		}
	}
}

// Called every frame


