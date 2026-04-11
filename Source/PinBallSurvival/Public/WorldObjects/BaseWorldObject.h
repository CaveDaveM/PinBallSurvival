// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWorldObject.generated.h"

class USphereComponent;

UCLASS()
class PINBALLSURVIVAL_API ABaseWorldObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWorldObject();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* HealthMesh;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
