// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GOAPAIController.generated.h"

/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API AGOAPAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGOAPAIController();
protected:
	void MovePawn(FVector NewLocation);
	
};
