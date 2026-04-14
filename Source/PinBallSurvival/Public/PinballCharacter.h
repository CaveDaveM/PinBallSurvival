// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatantBaseClass.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "PlayerProgressionSubsystem.h"
#include "GameFramework/Pawn.h"
#include "PinballCharacter.generated.h"

class APlayerHUD;
class ABasicProjectile;
class UProjectileWeapons;
class USphereComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputAction;

UCLASS()
class PINBALLSURVIVAL_API APinballCharacter : public ACombatantBaseClass
{
	GENERATED_BODY()

public:
	APinballCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void ApplyHealing(float HealAmount) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UStaticMeshComponent* PlayerMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	USpringArmComponent* SpringArm;
	
	UFUNCTION()
	void ApplyForceToPlayer(FVector ForceToApply);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> FirstPersonContext;
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> GameMenuAction;
	
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UPlayerProgressionSubsystem* PlayerProgression;
	
	UPROPERTY()
	APlayerHUD* HUD;
	
	//movement and inputs
	UFUNCTION(Category="Input")
	void MoveInput(const FInputActionValue& Value);
	UFUNCTION(Category="Input")
	virtual void DoMove(FVector2D MoveVector);
	UFUNCTION(Category="Input")
	void OpenGameMenu();
	
	//PlayerStats update
	UFUNCTION()
	void UpdatePlayerStats(FPlayerStats NewPlayerStats); 
	void UpdateCurrentSpeed();
	void CalculateDamage();
	
	FTimerHandle UpdateCurrentSpeed_TimeHandle;
	float CurrentSpeed;
	float DamageScaling = 0.1f;
	FPlayerStats PlayerStats;
	float Health = 100.0f;
	
};
