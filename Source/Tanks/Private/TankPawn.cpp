// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "TanksPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainMenuWidgetCPP.h"
#include "Animation/Rig.h"
#include "Components/ProgressBar.h"
#include "Tanks/Tanks.h"


// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	RootComponent = BoxComponent;

	BoxComponent2 = CreateDefaultSubobject<UBoxComponent>("TrackLeft");
	BoxComponent2->SetupAttachment(RootComponent);

	BoxComponent3 = CreateDefaultSubobject<UBoxComponent>("TrackRight");
	BoxComponent3->SetupAttachment(RootComponent);

	//BoxComponent4 = CreateDefaultSubobject<UBoxComponent>("Tower");
	//BoxComponent4->SetupAttachment(RootComponent);

	//BoxComponent4 = CreateDefaultSubobject<UBoxComponent>("Cannon");
	//BoxComponent4->SetupAttachment(BoxComponent4);

	TankBody = CreateDefaultSubobject<UStaticMeshComponent>("TankBody");
	TankBody->SetupAttachment(RootComponent);

	TankTurret = CreateDefaultSubobject<UStaticMeshComponent>("TankTurret");
	TankTurret->SetupAttachment(TankBody);

	CannonPosition = CreateDefaultSubobject<UArrowComponent>("CannonPosition");
	CannonPosition->SetupAttachment(TankTurret);

	//HealthWidget = CreateDefaultSubobject<UProgressBar>("HealthBar");
	//HealthWidget = 
	//HealthBar =
	

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>("ArmComponent");
	ArmComponent->SetupAttachment(RootComponent);
	ArmComponent->bInheritPitch = false;
	ArmComponent->bInheritRoll = false;
	ArmComponent->bInheritYaw = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(ArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	
	//UWidgetComponent* WidgetComp = CreateDefaultSubobject<UWidgetComponent>("WidgetComp");
	//WidgetComp->SetupAttachment(RootComponent);
	//WidgetComp->SetWidgetClass(HealthWidget);
	
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

int ATankPawn::GetClipContains()
{
	return ClipContains;
}


// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATanksPlayerController>(GetController());

	if (CannonType)
	{
		auto Transform = CannonPosition->GetComponentTransform();
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonType, Transform);
		Cannon->AttachToComponent(CannonPosition, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
	
	
	
	AmmoLeft = AmmoStockSize;
	ClipContains = ClipSize;
	//GEngine->AddOnScreenDebugMessage(8888, 10000, FColor::Yellow, FString(TEXT("Ammo left: " + FString::FromInt(AmmoLeft))));
	//GEngine->AddOnScreenDebugMessage(7777, 10000, FColor::Yellow, FString(TEXT("Clip ammo: " + FString::FromInt(ClipContains))));
	HealthPointsCurrent = HealthPointsMax;
	//HealthWidget->UpdateHealthBar(HealthPointsCurrent, HealthPointsMax);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveTank(DeltaTime);
	RotateTank(DeltaTime);
	RotateCannon(DeltaTime);
	FireRateTimerValue = Cannon->GetTimerValue();
	//HealthWidget->UpdateHealthBar(HealthPointsCurrent, HealthPointsMax);
	//GEngine->AddOnScreenDebugMessage(2222, -1, FColor::Magenta, FString::Printf(TEXT("%f"), GetWorld()->GetTimerManager().GetTimerElapsed(TimerHandle)));
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::MoveTank(float DeltaTime)
{
	CurrentForwardMoveScale = FMath::Lerp(CurrentForwardMoveScale, TargetForwardPushScale, MovementAcceleration);
	const auto Location = GetActorLocation();
	SetActorLocation(Location + GetActorForwardVector() * MovementSpeed * CurrentForwardMoveScale * DeltaTime + GetActorRightVector() * MovementSpeed * RightPushScale * DeltaTime, false);
	//GEngine->AddOnScreenDebugMessage(1232, 10, FColor::Blue, FString::Printf(TEXT("%f"), CurrentForwardMoveScale));
}

void ATankPawn::RotateTank(float DeltaTime)
{
	CurrentRotationScale = FMath::Lerp(CurrentRotationScale, TargetRotationScale, RotationAcceleration);
	auto Rotation = GetActorRotation();
	Rotation.Yaw += CurrentRotationScale * RotationSpeed * DeltaTime;
	SetActorRotation(Rotation);
	//UE_LOG(LogTanks, Warning, TEXT("%s"), *Rotation.ToString());
}

void ATankPawn::RotateCannon(float Deltatime)
{
	if(!TankController) return;
	//const auto TankOrientation = GetActorRotation();
	//auto CurrentRotation = TankTurret->GetComponentRotation();
	//FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TankTurret->GetComponentLocation(), TankController->GetMousePosition());
	//TargetRotation.Yaw += 3.14f - GetActorRotation().Yaw;
	//TankTurret->SetRelativeRotation(FMath::Lerp(TankTurret->GetComponentRotation(), TargetRotation, RotationAccelerationTurret));
	//TargetRotation = TankTurret->GetComponentRotation();
	//TargetRotation.Pitch = 0;
	//TargetRotation.Roll = 0;
	//TankTurret->SetRelativeRotation(TargetRotation);
	FRotator Difference = GetActorRotation()-UKismetMathLibrary::FindLookAtRotation(TankTurret->GetComponentLocation(), TankController->GetMousePosition());
	FRotator RelativeTargetRotation;
	RelativeTargetRotation.Pitch = 0;
	RelativeTargetRotation.Roll = 0;
	RelativeTargetRotation.Yaw = 3.14-Difference.Yaw;
	TankTurret->SetRelativeRotation(RelativeTargetRotation);
	//FRotator CurrentRotation;
	//FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TankTurret->GetComponentLocation(), TankController->GetMousePosition());
	//CurrentRotation.Pitch = GetActorRotation().Pitch - TankTurret->GetComponentRotation().Pitch;
	//CurrentRotation.Roll = GetActorRotation().Roll - TankTurret->GetComponentRotation().Roll;
	//CurrentRotation.Yaw = TargetRotation.Yaw;
	//TankTurret

	//CurrentRotation = TankTurret->GetComponentRotation();
	//CurrentRotation.Pitch = 0;
	//CurrentRotation.Roll = 0;
	//TankTurret->SetRelativeRotation(CurrentRotation);
	
	//TankTurret->SetWorldRotation(FMath::Lerp(TankTurret->GetComponentRotation(), CurrentRotation, RotationAccelerationTurret));
	
	
	
}

void ATankPawn::ChangeFireMode()
{
	if (Cannon)
		Cannon->CycleFireMode();
}

void ATankPawn::Reload(int ReloadAmount)
{
	bReloaded = false;
	ClipContains = ReloadAmount;
	AmmoLeft -= ReloadAmount;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this,&ATankPawn::ResetReloadState), 1/ReloadRate, false, ReloadRate);
}

void ATankPawn::ResetReloadState()
{
	bReloaded = true;
}

void ATankPawn::Shoot()
{
	if (Cannon)
	{
		if (!bReloaded) return;
		if (ClipContains > 0)
		{
			int ShootAmount = 1;
			if (!Cannon->Shoot(ClipContains)) return;
			if (Cannon->Type == ECannonType::FireAuto) ShootAmount = ClipContains;
			ClipContains -= ShootAmount;
			//GEngine->AddOnScreenDebugMessage(7777, 10000, FColor::Yellow, FString(TEXT("Clip ammo: " + FString::FromInt(ClipContains))));
		}
		else if (AmmoLeft > 0)
		{
			if (AmmoLeft > ClipSize)
			{
				Reload(ClipSize);
			}
				
			else
			{
				Reload(AmmoLeft);
			}
			
		}
		else
		{
			bReloaded = false;
		}
			
		
	}
}


