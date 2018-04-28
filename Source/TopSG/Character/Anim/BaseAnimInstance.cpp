// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAnimInstance.h"
#include "Character/TopSGCharacter.h"


void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ATopSGCharacter* owner = Cast<ATopSGCharacter>(TryGetPawnOwner());
	if (owner)
	{
		Speed = owner->GetVelocity().Size();
	}
}
