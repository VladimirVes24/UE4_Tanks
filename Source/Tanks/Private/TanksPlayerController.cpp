// Fill out your copyright notice in the Description page of Project Settings.


#include "TanksPlayerController.h"

//#include "DrawDebugHelpers.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"

void ATanksPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//Binding axis "MoveForward" from UE4 to call OnMoveForward
	InputComponent->BindAxis("MoveForward", this, &ATanksPlayerController::OnTryingToMoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATanksPlayerController::OnTryingToMoveRight);
	InputComponent->BindAxis("RotationRight", this, &ATanksPlayerController::OnTryingToRotateRight);
	InputComponent->BindAction("Shoot", IE_Pressed, this, &ATanksPlayerController::OnShoot);
	InputComponent->BindAction("Shoot", IE_Released, this, &ATanksPlayerController::OnShootStop);
	InputComponent->BindAction("CycleFireMode", IE_Pressed, this, &ATanksPlayerController::OnTryingToChangeFireRate);
	InputComponent->BindAction("Flip", IE_Pressed, this, &ATanksPlayerController::OnFlip);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this, &ATanksPlayerController::OnShootSpecial);
	InputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &ATanksPlayerController::OnTryingToSwitchWeapon);

	bShowMouseCursor = true;
}

/**
 * @brief this function is called when "W" or "S" is pressed (as set in UE4) 
 * @param Scale parameter from UE4, passed when function is called
 */
void ATanksPlayerController::OnTryingToMoveForward(float InputPushScale)
{
	if (PlayerTank)
		PlayerTank->SetMoveForwardPushScale(InputPushScale);
}

void ATanksPlayerController::OnTryingToMoveRight(float InputPushScale)
{
	if (PlayerTank)
		PlayerTank->SetMoveRightPushScale(InputPushScale);
}

void ATanksPlayerController::OnTryingToRotateRight(float InputPushScale)
{
	if (PlayerTank)
		PlayerTank->SetRotateRightScale(InputPushScale);
}

void ATanksPlayerController::OnTryingToChangeFireRate()
{
	if (PlayerTank)
		PlayerTank->ChangeFireMode();
}

void ATanksPlayerController::OnTryingToSwitchWeapon()
{
	if (PlayerTank)
		PlayerTank->SwitchWeapon();
}

void ATanksPlayerController::OnShoot()
{
	if (PlayerTank)
		PlayerTank->Shoot();
}

void ATanksPlayerController::OnShootStop()
{
	if (PlayerTank)
		PlayerTank->OnShootStop();
}


void ATanksPlayerController::OnShootSpecial()
{
	if (PlayerTank)
		PlayerTank->FireSpecial();
}

void ATanksPlayerController::OnFlip()
{
	if (PlayerTank)
		PlayerTank->Flip();
}


void ATanksPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerTank = CastChecked<ATankPawn>(GetPawn());
}

void ATanksPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector MousePosition, MouseDirection;
	DeprojectMousePositionToWorld(MousePosition, MouseDirection);

	auto Z = FMath::Abs(MousePosition.Z - PlayerTank->CannonPosition->GetComponentLocation().Z);
	MouseWorldPosition = MousePosition - MouseDirection * Z / MouseDirection.Z;
	
	//DrawDebugSphere(GetWorld(), MousePosition, 3, 16, FColor::Red);
	DrawDebugLine(GetWorld(), PlayerTank->CannonPosition->GetComponentLocation(), MouseWorldPosition, FColor::Black, false, -1, 0, 10);

	//DrawDebugSphere(GetWorld(), MouseWorldPosition, 3, 16, FColor::Green);

	//FHitResult Result;
	//GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Result);
	//DrawDebugSphere(GetWorld(), Result.ImpactPoint, 3, 16, FColor::Blue);
	
}
