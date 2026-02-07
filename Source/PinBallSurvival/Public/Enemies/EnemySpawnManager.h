// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnManager.generated.h"

class UBoxComponent;

UCLASS()
class PINBALLSURVIVAL_API AEnemySpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnManager();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** At this stage i cant figure out how to get the extends of the box and transform them into coordinates.
	 * I will use a Hard Set approach for now **/
	
	UPROPERTY(EditInstanceOnly, Category = "SpawnBounds")
	FVector BoundsLocationX;
	
	UPROPERTY(EditInstanceOnly, Category = "SpawnBounds")
	FVector BoundsLocationY;
	
	UPROPERTY(EditAnywhere, Category = "Bounds")
	TObjectPtr<UBoxComponent> EnemySpawnBounds;

};
