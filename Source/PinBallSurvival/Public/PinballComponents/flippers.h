// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "flippers.generated.h"

class UArrowComponent;
DECLARE_LOG_CATEGORY_EXTERN(GameInfo, Display, All);

class APinballCharacter;
class UBoxComponent;

UCLASS()
class PINBALLSURVIVAL_API Aflippers : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aflippers();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
protected:
	virtual void BeginPlay() override;
	
	// testing TobjectPtr for this file. seeing if it works or what it does
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Force")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "OverlapComponent")
	TObjectPtr<UBoxComponent> BoxOverlapComp;
	
	UPROPERTY()
	TObjectPtr<APinballCharacter> PlayerCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category = "PushDirectionArrows")
	TObjectPtr<UArrowComponent> ArrowComponent;
	
	
	//found on unreal garden properties, quite cool really 
	UPROPERTY(EditInstanceOnly, Category = "Force", meta = (Units="Kilograms"))
	float PushStrength = 10000.0f;
	
	UFUNCTION(BlueprintCallable, Category = "Pushback")
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable, Category = "Pushback")
	void ApplyForceToPlayerLocal();


private:
};
