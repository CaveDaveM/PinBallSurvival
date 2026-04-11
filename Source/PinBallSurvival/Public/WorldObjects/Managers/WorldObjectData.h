// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WorldObjectData.generated.h"

class ABaseWorldObject;

UENUM()
enum class EObjectRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
	Unique UMETA(DisplayName = "Unique"),
	
};

USTRUCT()
struct FWorldObjectData 
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWorldObject> BaseWorldObject;
	
	UPROPERTY(EditAnywhere)
	EObjectRarity Rarity;
	
};



