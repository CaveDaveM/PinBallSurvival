// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "GameFramework/Pawn.h"
#include "PinballCharacter.generated.h"

class USpringArmComponent;
struct FInputActionValue;
class UInputAction;

UCLASS()
class PINBALLSURVIVAL_API APinballCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APinballCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UStaticMeshComponent* PlayerMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	USpringArmComponent* SpringArm;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Movement
	UFUNCTION()
	void MoveInput(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(FVector2D MoveVector);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> FirstPersonContext;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	//Updating The Current Speed of the player
	UPROPERTY()
	FTimerHandle UpdateCurrentSpeed_TimeHandle;
	
	UPROPERTY()
	FVector CurrentSpeed;
	
	UFUNCTION()
	void UpdateCurrentSpeed();
	
};
