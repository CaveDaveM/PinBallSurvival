// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "EnemyBaseClass.generated.h"

UCLASS()
class PINBALLSURVIVAL_API AEnemyBaseClass : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBaseClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Mesh")
	UStaticMeshComponent* StaticMeshComponent;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
