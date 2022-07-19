// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "TanksPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainMenuWidgetCPP.h"
#include "Animation/Rig.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
    HealthComponent->OnDie.AddUObject(this, &ATankPawn::Die);
    HealthComponent->OnDamaged.AddUObject(this, &ATankPawn::DamageTaken);
	
	TankBodyCollisionBox = CreateDefaultSubobject<UBoxComponent>("Body Collision");
	RootComponent = TankBodyCollisionBox;

	LeftTrackCollisionBox = CreateDefaultSubobject<UBoxComponent>("Left Track Collision");
	LeftTrackCollisionBox->SetupAttachment(RootComponent);

	RightTrackCollisionBox = CreateDefaultSubobject<UBoxComponent>("Right Track Collision");
	RightTrackCollisionBox->SetupAttachment(RootComponent);
	
	TankBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Tank Body Mesh");
	TankBodyMesh->SetupAttachment(RootComponent);

	TankTowerMesh = CreateDefaultSubobject<UStaticMeshComponent>("Tank Tower Mesh");
	TankTowerMesh->SetupAttachment(TankBodyMesh);

	TankTowerCollisionBox = CreateDefaultSubobject<UBoxComponent>("Tank Tower Collision");
	TankTowerCollisionBox->AttachToComponent(TankTowerMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonPosition = CreateDefaultSubobject<UArrowComponent>("Cannon Position");
	CannonPosition->SetupAttachment(TankTowerMesh);

	MachineGunPosition = CreateDefaultSubobject<UArrowComponent>("MachineGun Position");
	MachineGunPosition->SetupAttachment(TankTowerMesh);

	CannonCollisionBox = CreateDefaultSubobject<UBoxComponent>("Tank Cannon Collision");
	CannonCollisionBox->AttachToComponent(CannonPosition, FAttachmentTransformRules::KeepRelativeTransform);

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>("ArmComponent");
	ArmComponent->SetupAttachment(RootComponent);
	ArmComponent->bInheritPitch = false;
	ArmComponent->bInheritRoll = false;
	ArmComponent->bInheritYaw = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(ArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	DeathExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>("Explosion of death");
	DeathExplosionEffect->SetupAttachment(RootComponent);

	

	//ShootShake = CreateDefaultSubobject<UMatineeCameraShake>("CameraShakeEffect");
}

/**
 * @brief function for getting how hard button is pressed (e.g. on gamepad) and saving it for speed correction later
 * @param Scale scale of input (how hard button is pressed)
 */
void ATankPawn::SetMoveForwardPushScale(float Scale)
{
	TargetForwardPushScale = Scale;
}

void ATankPawn::SetMoveRightPushScale(float Scale)
{
	RightPushScale = Scale;
}

void ATankPawn::SetRotateRightScale(float Scale)
{
	TargetRotationScale = Scale;
}

void ATankPawn::SetRotateTowerRightScale(float Scale)
{
	TargetRotationScaleTurret = Scale;
}

int ATankPawn::GetClipContains() const
{
	return ClipContains;
}


// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	DeathExplosionEffect->DeactivateSystem();

	TankController = Cast<ATanksPlayerController>(GetController());
	
	SetupCannon(CannonType);
	SetupMachineGun(MachineGunType);

	TotalScore = 0;
	IncreaseTotalScore(0);
	AmmoLeft = AmmoStockSize;
	ClipContains = ClipSize;
	HealthPointsCurrent = HealthPointsMax;
	CurrentHealthState = HealthComponent->GetHealthState();
}

void ATankPawn::Destroyed()
{
	DeathExplosionEffect->ActivateSystem();	
	if (Cannon) Cannon->Destroy();
	if (MachineGun) MachineGun->Destroy();
	Super::Destroyed();
}

void ATankPawn::Die()
{
	Destroy();
}

void ATankPawn::ShakeCamera()
{
	//if(GetOwner() && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
    //{
		if(ShootShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(ShootShake);
		}
    //}

}

void ATankPawn::DamageTaken(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
	CurrentHealthState = HealthComponent->GetHealthState();
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (FMath::UnwindDegrees(FMath::Abs(GetActorRotation().Roll) >= 120)) bFlipped = true;
	else bFlipped = false;
	
	MoveTank(DeltaTime);
	RotateTank(DeltaTime);
	//RotateCannon();
	TiltCannon();
	if(TankController)
	{
		FVector mousePos = TankController->GetMousePosition();
		RotateCannon(mousePos);
		//RotateCannonViaGamepad(DeltaTime);
	}
	
	if (Cannon) FireRateTimerValue = Cannon->GetTimerValue();
	GEngine->AddOnScreenDebugMessage(7777, 1, FColor::Blue, FString(TEXT("Clip ammo: " + FString::FromInt(ClipContains))));
	GEngine->AddOnScreenDebugMessage(8888, 1, FColor::Blue, FString(TEXT("Ammo left: " + FString::FromInt(AmmoLeft))));
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TankTowerMesh->GetForwardVector();
}

TArray<FVector> ATankPawn::GetPatrollingPoints()
{
	TArray<FVector> points;
    for (ATargetPoint* point : PatrollingPoints)
    {
		points.Add(point->GetActorLocation());
    }
    return points;
}

void ATankPawn::SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints)
{
	PatrollingPoints = NewPatrollingPoints;
}

void ATankPawn::MoveTank(float DeltaTime)
{
	if (bFlipped) return;
	CurrentForwardMoveScale = FMath::Lerp(CurrentForwardMoveScale, TargetForwardPushScale, MovementAcceleration);
	const auto Location = GetActorLocation();
	SetActorLocation(Location + GetActorForwardVector() * MovementSpeed * CurrentForwardMoveScale * DeltaTime + GetActorRightVector() * MovementSpeed * RightPushScale * DeltaTime, false);
	//GEngine->AddOnScreenDebugMessage(1232, 10, FColor::Blue, FString::Printf(TEXT("%f"), CurrentForwardMoveScale));
}

void ATankPawn::RotateTank(float DeltaTime)
{
	if (bFlipped) return;
	CurrentRotationScale = FMath::Lerp(CurrentRotationScale, TargetRotationScale, RotationAcceleration);
	auto Rotation = GetActorRotation();
	Rotation.Yaw += CurrentRotationScale * RotationSpeed * DeltaTime;
	SetActorRotation(Rotation);
	//UE_LOG(LogTanks, Warning, TEXT("%s"), *Rotation.ToString());
}

void ATankPawn::RotateCannon(FVector TargetPosition)
{
	//if(!TankController) return;
	if (bFlipped) return;
	
	const FRotator ActorAbsoluteRotation = GetActorRotation();
	const FRotator TurretAbsoluteRotation = CannonPosition->GetComponentRotation();
	const FRotator TurretRelativeRotation = TurretAbsoluteRotation - ActorAbsoluteRotation;
	
	const auto CurrentDifference = UKismetMathLibrary::FindLookAtRotation(TankTowerMesh->GetComponentLocation(), TargetPosition) - TurretAbsoluteRotation;

	const float TurretTargetRelativeRotationYaw = TurretRelativeRotation.Yaw + FMath::UnwindDegrees(CurrentDifference.Yaw);
	const float YawRotation = TurretTargetRelativeRotationYaw;
	const FRotator NewRotation = FRotator(0, YawRotation, 0);
	TankTowerMesh->SetRelativeRotation(NewRotation);
}

void ATankPawn::RotateCannonViaGamepad(float DeltaTime)
{
	if (bFlipped) return;
	//CurrentRotationScaleTurret = FMath::Lerp(CurrentRotationScaleTurret, TargetRotationScaleTurret, RotationAccelerationTurret);
	//auto AbsoluteTurretRotation = TankTowerMesh->GetComponentRotation();
	//AbsoluteTurretRotation.Yaw += CurrentRotationScaleTurret * DeltaTime;
	//TankTowerMesh->SetRelativeRotation(FRotator(0, AbsoluteTurretRotation.Yaw-GetActorRotation().Yaw, 0));

	FRotator ActorAbsoluteRotation = GetActorRotation();
	FRotator TurretAbsoluteRotation = CannonPosition->GetComponentRotation();
	FRotator TurretRelativeRotation = TurretAbsoluteRotation - ActorAbsoluteRotation;
	TurretRelativeRotation.Yaw += TargetRotationScaleTurret*RotationSpeedTurret*DeltaTime;
	FRotator NewRotation = FRotator(0, TurretRelativeRotation.Yaw, 0);
	TankTowerMesh->SetRelativeRotation(NewRotation);
}
/*
void ATankPawn::RotateCannonTo(FVector TargetPosition)
{
	FRotator targetRotation =
    UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
    FRotator currRotation = TankTowerMesh->GetComponentRotation();
    targetRotation.Pitch = currRotation.Pitch;
    targetRotation.Roll = currRotation.Roll;
    TankTowerMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, RotationAccelerationTurret));
}
*/
void ATankPawn::TiltCannon() const
{
	if(!TankController) return;
	if (bFlipped) return;

	const FRotator ActorAbsoluteRotation = GetActorRotation();
	const FRotator CannonAbsoluteRotation = CannonPosition->GetComponentRotation();
	const FRotator CannonRelativeRotation = CannonAbsoluteRotation - ActorAbsoluteRotation;
	//const FRotator CurrentDifference = UKismetMathLibrary::FindLookAtRotation(TankTowerMesh->GetComponentLocation(), TankController->GetMousePosition()) - CannonAbsoluteRotation;
	const FRotator CurrentDifference = UKismetMathLibrary::FindLookAtRotation(CannonPosition->GetComponentLocation(), TankController->GetMousePosition()) - CannonAbsoluteRotation;

	FRotator NewRotation = CannonRelativeRotation;
	if (CurrentDifference.Pitch == 0) return;
	if (FMath::Abs(NewRotation.Pitch + CurrentDifference.Pitch) >= CannonTiltMaxDegree)
	{
		if (CurrentDifference.Pitch <0) NewRotation.Pitch = -CannonTiltMaxDegree;
		else NewRotation.Pitch = CannonTiltMaxDegree;
	}
	else NewRotation.Pitch += CurrentDifference.Pitch;

	CannonPosition->SetRelativeRotation(FRotator(NewRotation.Pitch, 0, 0 ));
	
}

void ATankPawn::SwitchWeapon()
{
	bSwitchedToSecondary = !bSwitchedToSecondary;
	if (bSwitchedToSecondary) GEngine->AddOnScreenDebugMessage(8238, 10, FColor::White, FString::Printf(TEXT("Switched to Machine Gun")));
	else GEngine->AddOnScreenDebugMessage(8238, 10, FColor::White, FString::Printf(TEXT("Switched to Cannon")));
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> DesiredCannonType)
{
	if (DesiredCannonType) { CannonType = DesiredCannonType; }
	if (Cannon) { Cannon->Destroy(); }
	
	const auto Transform = CannonPosition->GetComponentTransform();
	FActorSpawnParameters CannonSpawnParams;
	CannonSpawnParams.Instigator = this;
	CannonSpawnParams.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonType, Transform, CannonSpawnParams);
	Cannon->AttachToComponent(CannonPosition, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ATankPawn::SetupMachineGun(TSubclassOf<ACannon> DesiredMachineGunType)
{
	if (DesiredMachineGunType) { MachineGunType = DesiredMachineGunType; }
	if (MachineGun) { MachineGun->Destroy(); }
	
	const auto Transform = MachineGunPosition->GetComponentTransform();
	FActorSpawnParameters MachineGunSpawnParams;
	MachineGunSpawnParams.Instigator = this;
	MachineGunSpawnParams.Owner = this;
	MachineGun = GetWorld()->SpawnActor<ACannon>(MachineGunType, Transform, MachineGunSpawnParams);
	MachineGun->AttachToComponent(MachineGunPosition, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ATankPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
	ShakeCamera();
}

void ATankPawn::IncreaseTotalScore(int InputScore)
{
	TotalScore+=InputScore;
	GEngine->AddOnScreenDebugMessage(8585, 10000, FColor::Yellow, FString(TEXT("Total score: " + FString::FromInt(TotalScore))));
	
}

FVector ATankPawn::GetEyesPosition()
{
	return CannonPosition->GetComponentLocation();
}

void ATankPawn::ChangeFireMode() const
{
	if (Cannon)
		Cannon->CycleFireMode();
}

void ATankPawn::Reload(int ReloadAmount)
{
	bReloaded = false;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([=]()
	{
		ResetReloadState(ReloadAmount);
	}
	);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1/ReloadRate, false, ReloadRate);
}

void ATankPawn::Flip()
{
	if (!bFlipped) return;
	const auto Location = GetActorLocation();
	const FVector ChangeLoc = {0,0,100};
	const FRotator ChangeRot = {0,0,0};
	SetActorRotation(ChangeRot);
	SetActorLocation(Location + ChangeLoc);
	bFlipped = false;
}

void ATankPawn::ResetReloadState(int ReloadAmount)
{
	bReloaded = true;
	ClipContains = ReloadAmount;
	AmmoLeft -= ReloadAmount;
}

void ATankPawn::Shoot()
{
	if (bSwitchedToSecondary && MachineGun)
	{
		
		GetWorld()->GetTimerManager().SetTimer(MachineGunTimerHandle, FTimerDelegate::CreateUObject(this, &ATankPawn::FireMachineGun), 1/MachineGun->FireRate, true);
	}
	else if (!bSwitchedToSecondary && Cannon)
	{
		FireCannon();
	}
}

void ATankPawn::FireCannon()
{
	if (!bReloaded) return;
	if (ClipContains > 0)
	{
		int ShootAmount = 1;
		if (Cannon->Type == ECannonType::FireAuto && Cannon->BurstSize >= ClipContains) ShootAmount = ClipContains;
		if (Cannon->Type == ECannonType::FireAuto && Cannon->BurstSize < ClipContains) ShootAmount = Cannon->BurstSize;
		if (Cannon->Type == ECannonType::FireTrace) ShootAmount = 0;
		if (!Cannon->Shoot(ShootAmount)) return;
		Cannon->GetLastProjectilePointer()->OnKill.AddUObject(this, &ATankPawn::IncreaseTotalScore);
		ClipContains -= ShootAmount;
		//GEngine->AddOnScreenDebugMessage(7777, 10000, FColor::Yellow, FString(TEXT("Clip ammo: " + FString::FromInt(ClipContains))));
	}
	if (ClipContains <=0 && AmmoLeft > 0)
	{
		bReloaded = false;
		if (AmmoLeft > ClipSize)
		{
			Reload(ClipSize);
		}
				
		else
		{
			Reload(AmmoLeft);
		}
	}
}

void ATankPawn::FireMachineGun()
{
	if (MachineGun->Type == ECannonType::FireMachineGun)
	{
		MachineGun->Shoot(1);
	}
}

void ATankPawn::OnShootStop()
{
	if (!bSwitchedToSecondary && Cannon && (Cannon->Type == ECannonType::FireTrace))
	{
		Cannon->StopTrace();
	}
	else if (bSwitchedToSecondary && MachineGun) GetWorldTimerManager().ClearTimer(MachineGunTimerHandle);
}



void ATankPawn::FireSpecial()
{
	if (Cannon->Type == ECannonType::FireAuto)
	{
		Shoot();
	}
	else
	{
		const ECannonType OldCannonType = Cannon->Type;
		Cannon->Type = ECannonType::FireAuto;
		Shoot();
		Cannon->Type = OldCannonType;
	}
}




