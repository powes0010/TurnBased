// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomStruct.generated.h"

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
		float HP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
		float ATK;
};