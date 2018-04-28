// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseBattleLevelActor.h"
#include "TopSG/Character/TopSGCharacter.h"
#include "TopSG/Character/BaseBattlePawn/BaseBattlePawn.h"
#include "TopSG/Character/BaseEnemy/BaseEnemy.h"
#include "TopSG/Controller/TopSGPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"


const int32  SeatingNum = 6;

// Sets default values
ABaseBattleLevelActor::ABaseBattleLevelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	LevelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LevelMesh"));
	LevelMesh->SetRelativeLocation(FVector(0.f, 0.f, -5.f));
	LevelMesh->SetRelativeScale3D(FVector(3.f));
	LevelMesh->SetupAttachment(RootComponent);


	BattleLevelCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BattleLevelCamera"));
	BattleLevelCamera->SetupAttachment(RootComponent);
	BattleLevelCamera->SetRelativeRotation(FRotator(-30.f, -90.f, 0.f));
	BattleLevelCamera->SetRelativeLocation(FVector(0.f, 300.f, 260.f));

	LevelType = EBattleLevelType::EBT_BiWuTai;

	RedArrowLoc.Add(FVector(-130.f, 0.f, 10.f));
	RedArrowLoc.Add(FVector(-130.f, -130.f, 10.f));
	RedArrowLoc.Add(FVector(-130.f, 130.f, 10.f));
	RedArrowLoc.Add(FVector(-230.f, 0.f, 10.f));
	RedArrowLoc.Add(FVector(-230.f, -130.f, 10.f));
	RedArrowLoc.Add(FVector(-230.f, 130.f, 10.f));

	BlueArrowLoc.Add(FVector(130.f, 0.f, 10.f));
	BlueArrowLoc.Add(FVector(130.f, -130.f, 10.f));
	BlueArrowLoc.Add(FVector(130.f, 130.f, 10.f));
	BlueArrowLoc.Add(FVector(230.f, 0.f, 10.f));
	BlueArrowLoc.Add(FVector(230.f, -130.f, 10.f));
	BlueArrowLoc.Add(FVector(230.f, 130.f, 10.f));

	for (int32 i = 0 ; i< RedArrowLoc.Num(); i++)
	{
		UArrowComponent* arrow = CreateDefaultSubobject<UArrowComponent>(*FString::Printf(TEXT("RedArrow_%d"), i));
		arrow->SetupAttachment(RootComponent);
		arrow->SetRelativeLocation(RedArrowLoc[i]);		
		RedGroup.Add(arrow);
	}
	for (int32 i = 0; i < BlueArrowLoc.Num(); i++)
	{
		UArrowComponent* arrow = CreateDefaultSubobject<UArrowComponent>(*FString::Printf(TEXT("BlueArrow_%d"), i));
		arrow->SetupAttachment(RootComponent);
		arrow->SetRelativeLocation(BlueArrowLoc[i]);
		arrow->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
		BlueGroup.Add(arrow);
	}
}

// Called when the game starts or when spawned
void ABaseBattleLevelActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseBattleLevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BattleTick(DeltaTime);
}

void ABaseBattleLevelActor::BattleStart(class ATopSGCharacter* player, class ABaseEnemy* enemy)
{
	if (player && enemy)
	{
		Player = player;
		Enemy = enemy;

		ATopSGPlayerController* TSC = Cast<ATopSGPlayerController>(GetWorld()->GetFirstPlayerController());
		if (TSC)
		{
			TSC->bIsInBattleLevel = true;

			for (int32 i = 0; i< FMath::Min<int32>(Player->GetPartners().Num(), SeatingNum); i++)
			{
				if (Player->GetPartners()[i].PartnerClass && RedGroup.Num()>i && RedGroup[i])
				{
					UClass* pawnclass = Player->GetPartners()[i].PartnerClass;
					ABaseBattlePawn* btp = GetWorld()->SpawnActor<ABaseBattlePawn>(pawnclass, RedGroup[i]->GetComponentTransform());
					if (btp)
					{
						PlayerPawns[btp] = FPawnMsg();
					}
				}
			}

			for (int32 i = 0; i< FMath::Min<int32>(Enemy->GetAllEnemy().Num(), SeatingNum); i++)
			{
				if (Enemy->GetAllEnemy()[i]&& BlueGroup.Num() > i && BlueGroup[i])
				{
					UClass* pawnclass = Enemy->GetAllEnemy()[i];
					ABaseBattlePawn* btp = GetWorld()->SpawnActor<ABaseBattlePawn>(pawnclass, BlueGroup[i]->GetComponentTransform());
					if (btp)
					{
						EnemyPawns.Add(FBattlePawn(btp, i));
					}
				}
			}

			TSC->SetViewTarget(this);
		}
	}
}

void ABaseBattleLevelActor::BattleTick(float DeltaTime)
{
	if (bIsInBattleTick)
	{
		for (int32 i = 0; i < PlayerPawns.Num(); i++)
		{
			if (PlayerPawns[i].BattlePawn&& PlayerPawns[i].BattlePawn->HP > 0.f)
			{
				PlayerPawns[i].FightSeq -= PlayerPawns[i].BattlePawn->Speed * DeltaTime;
				if (PlayerPawns[i].FightSeq >= 100.f)
				{
					PlayerPawns[i].FightSeq = 0.f;
					bIsInBattleTick = false;
					break;
				}
			}
		}
	}
	if (bIsInBattleTick)
	{
		for (int32 i = 0; i < EnemyPawns.Num(); i++)
		{
			if (EnemyPawns[i].BattlePawn &&EnemyPawns[i].BattlePawn->HP > 0.f)
			{
				EnemyPawns[i].FightSeq -= EnemyPawns[i].BattlePawn->Speed * DeltaTime;
				if (EnemyPawns[i].FightSeq >= 100.f)
				{
					EnemyPawns[i].FightSeq = 0.f;
					bIsInBattleTick = false;
					break;
				}
			}			
		}
	}
	
}