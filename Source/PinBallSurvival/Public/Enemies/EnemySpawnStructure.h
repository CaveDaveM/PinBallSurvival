// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemySpawnStructure.generated.h"


//class AEnemyBase;

USTRUCT()
struct FWaveSection
{
	GENERATED_USTRUCT_BODY();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Enemy;

	UPROPERTY(EditAnywhere)
	int16 Amount = 1;

	UPROPERTY(EditAnywhere)
	float EnemySpawnInterval = 1.0f;
	
};

USTRUCT()
struct FWaveSpawnParams : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, Category = "WaveData")
	TArray<FWaveSection> Sections;
	
};