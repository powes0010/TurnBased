// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseBattlePawn.generated.h"

class UWidgetComponent;

UCLASS()
class TOPSG_API ABaseBattlePawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseBattlePawn();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* WidgetComComponent;

public:
	float HP = 100.f;

	float MaxHP = 100.f;

	float ATK = 10.f;

	float Speed = 100.f;
	
	
};
