// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"

#include "DrawDebugHelpers.h"
#include "TankPawn.h"

bool ATankAIController::IsPlayerSeen()
{
	if(!TankPawn) Initialize();

	FVector playerPos = PlayerPawn->GetActorLocation();
    FVector eyesPos = TankPawn->GetEyesPosition();
    FHitResult hitResult;
    FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
    traceParams.bTraceComplex = true;
    traceParams.AddIgnoredActor(TankPawn);
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

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(!TankPawn)
    Initialize();

	if(!TankPawn)
    return;

	TankPawn->SetMoveForwardPushScale(1);

	float RotationValue = GetRotationValue();
	TankPawn->SetRotateRightScale(RotationValue);

	Targeting();
}

float ATankAIController::GetRotationValue()
{
	FVector CurrentPointCoordinates = PatrollingPoints[CurrentPatrolPointIndex];
	FVector PawnLocation = TankPawn->GetActorLocation();

	if (FVector::Distance(PawnLocation, CurrentPointCoordinates) <= MovementAccuracy)
	{
		++CurrentPatrolPointIndex;
		if(CurrentPatrolPointIndex >= PatrollingPoints.Num())
			CurrentPatrolPointIndex = 0;
	}

	FVector MovementDirection = CurrentPointCoordinates - PawnLocation;
	MovementDirection.Normalize();
	FVector ForwardDirection = TankPawn->GetActorForwardVector();
	FVector RightDirection = TankPawn->GetActorRightVector();

	DrawDebugLine(GetWorld(), PawnLocation, CurrentPointCoordinates, FColor::Yellow, false, 0.1f, 0, 5);
	float ForwardAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ForwardDirection, MovementDirection)));
	float RightAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(RightDirection, MovementDirection)));

	float RotationValue = 0.0f;
	if(ForwardAngle > 5)
	{
		RotationValue = 1;
	}
	if (RightAngle > 90)
	{
		RotationValue = -RotationValue;
	}
	return RotationValue;
}

void ATankAIController::Targeting()
{
	
	if(CanFire() && IsPlayerInRange()) Fire();
	else RotateToPlayer();
}   

void ATankAIController::RotateToPlayer()
{
	if(IsPlayerInRange())
		TankPawn->RotateCannon(PlayerPawn->GetActorLocation());
}

bool ATankAIController::IsPlayerInRange()
{
	return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;

}

bool ATankAIController::CanFire()
{
	if (!IsPlayerSeen()) return false;
	FVector targetingDir = TankPawn->GetTurretForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	dirToPlayer.Normalize();
	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir,dirToPlayer)));
	return aimAngle <= Accuracy;
}

void ATankAIController::Fire()
{
	TankPawn->Shoot();
}

void ATankAIController::Initialize()
{
	TankPawn = Cast<ATankPawn>(GetPawn());
	PlayerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	FVector pawnLocation = TankPawn->GetActorLocation();
	MovementAccuracy = TankPawn->GetMovementAccuracy();
	TArray<FVector> points = TankPawn->GetPatrollingPoints();
	for(FVector point : points)
	{
		PatrollingPoints.Add(point + pawnLocation);
	}
	CurrentPatrolPointIndex = 0;
}


