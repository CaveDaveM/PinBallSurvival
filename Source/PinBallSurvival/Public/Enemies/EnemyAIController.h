// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UNavigationSystemV1;
/** this enemy class is will be only for simple enemy AI
 * only walks towards the player and attacks with close combat
 */
DECLARE_LOG_CATEGORY_EXTERN(EnemyAILOG,Display, All);

class AEnemyBaseClass;

UCLASS()
class PINBALLSURVIVAL_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void MoveEnemyToPlayer();
	
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	FVector GetPlayerLocation();

	UPROPERTY(EditDefaultsOnly)
	float WalkSpeed = 800.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	UNavigationSystemV1* NavSystem;
	
	FTimerHandle Action_TimeHandler;
	
	UPROPERTY()
	APawn* PlayerCharacterReference; 
	
	UPROPERTY()
	APawn* OwningPawn;
private:
};
