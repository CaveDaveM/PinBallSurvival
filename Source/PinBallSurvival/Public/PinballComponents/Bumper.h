// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bumper.generated.h"

class USphereComponent;

UCLASS()
class PINBALLSURVIVAL_API ABumper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABumper();
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BumperMesh;
	
	UPROPERTY(EditAnywhere)
	USphereComponent* BumperSphere;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	
	UFUNCTION()
	void PushAwayActor(APinballCharacter* OverlappedActor, FVector HitNormal);
	
	
	UFUNCTION()
	FVector ReflectVector(FVector Direction, FVector WallNormal, float Restitution);
	
	UPROPERTY(EditAnywhere)
	float PushStrength = 10.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
