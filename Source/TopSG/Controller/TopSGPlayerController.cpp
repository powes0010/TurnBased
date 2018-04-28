// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TopSGPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "TopSG/Character/TopSGCharacter.h"
#include "GameFramework/SpringArmComponent.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Character/TopSGCharacter.h"
#include "Engine/World.h"

ATopSGPlayerController::ATopSGPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ATopSGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void ATopSGPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATopSGPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ATopSGPlayerController::OnSetDestinationReleased);
	InputComponent->BindAction("ZoomOut", IE_Released, this, &ATopSGPlayerController::OnZommOut);
	InputComponent->BindAction("ZoomIn", IE_Released, this, &ATopSGPlayerController::OnZoomIn);

	InputComponent->BindAxis("MoveForward", this, &ATopSGPlayerController::MoveForword);
	InputComponent->BindAxis("MoveRight", this, &ATopSGPlayerController::MoveRight);


	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATopSGPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ATopSGPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &ATopSGPlayerController::OnResetVR);
}

void ATopSGPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATopSGPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (ATopSGCharacter* MyPawn = Cast<ATopSGCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UNavigationSystem::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ATopSGPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ATopSGPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 20.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ATopSGPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	if (!bIsInBattleLevel)
	{
		bMoveToMouseCursor = true;
	}	
}

void ATopSGPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	if (!bIsInBattleLevel)
	{
		bMoveToMouseCursor = false;
		StopMovement();
	}
}

void ATopSGPlayerController::OnZommOut()
{
	ATopSGCharacter* character = Cast<ATopSGCharacter>(GetPawn());
	if (character)
	{
		character->GetCameraBoom()->TargetArmLength = FMath::Min<float>(character->GetCameraBoom()->TargetArmLength + 10.f, 600.f);
	}
}

void ATopSGPlayerController::OnZoomIn()
{
	ATopSGCharacter* character = Cast<ATopSGCharacter>(GetPawn());
	if (character)
	{
		character->GetCameraBoom()->TargetArmLength = FMath::Max<float>(character->GetCameraBoom()->TargetArmLength - 10.f, 200.f);
	}
}

void ATopSGPlayerController::MoveForword(float value)
{
	if (!bMoveToMouseCursor && value!=0.f)
	{
		if (GetPawn())
		{
			const FRotator Rot = GetControlRotation();
			const FRotator YawRot = FRotator(0.f, Rot.Yaw, 0.f);
			const FVector Dir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);;
			GetPawn()->AddMovementInput(Dir, value);
		}	
	}
}
void ATopSGPlayerController::MoveRight(float value)
{
	if (!bMoveToMouseCursor && value != 0.f)
	{
		if (GetPawn())
		{
			const FRotator Rot = GetControlRotation();
			const FRotator YawRot = FRotator(0.f, Rot.Yaw, 0.f);
			const FVector Dir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);;
			GetPawn()->AddMovementInput(Dir,value);
		}
	}
}