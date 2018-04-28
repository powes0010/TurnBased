// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseBattlePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ABaseBattlePawn::ABaseBattlePawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(20.f, 40.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WidgetComComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidgetCom"));
	WidgetComComponent->SetupAttachment(GetCapsuleComponent());
	WidgetComComponent->SetRelativeLocation(FVector(0.f,0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()+ 10.f));
}

// Called when the game starts or when spawned
void ABaseBattlePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseBattlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
