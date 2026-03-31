// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemySpawnStructure.generated.h"


//class AEnemyBase;

class AMyPawn;

USTRUCT()
struct FWaveSection
{
	GENERATED_USTRUCT_BODY();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyPawn> Enemy;

	UPROPERTY(EditAnywhere)
	int16 Amount = 1;
	
};

USTRUCT()
struct FWaveSpawnParams : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, Category = "WaveData")
	TArray<FWaveSection> Sections;
	
};