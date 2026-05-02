// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatantBaseClass.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "NiagaraSystem.h"
#include "PlayerProgressionSubsystem.h"
#include "GameFramework/Pawn.h"
#include "Subsystems/WorldStateSubsystem.h"
#include "PinballCharacter.generated.h"

class UProgressBar;
class UAlwaysOnDisplay;
class UWidgetComponent;
class UTextBlock;
class APlayerHUD;
class ABasicProjectile;
class UProjectileWeapons;
class USphereComponent;
class USpringArmComponent;
class UInputAction;
struct FInputActionValue;

UCLASS()
class PINBALLSURVIVAL_API APinballCharacter : public ACombatantBaseClass
{
	GENERATED_BODY()

public:
	APinballCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void ApplyHealing(float HealAmount) override;
	virtual void ApplyDamage(float DamageAmount) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PlayerMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* PlayerHUDWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UAlwaysOnDisplay> AlwaysOnDisplayClass;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SpeedEffectClass;
	
	UFUNCTION()
	void ApplyForceToPlayer(FVector ForceToApply);
	

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> FirstPersonContext;
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> GameMenuAction;
	UPROPERTY() // keep a ref so GC doesn't eat it
	UAlwaysOnDisplay* AlwaysOnDisplayHud = nullptr;
	
	virtual void BeginPlay() override;
	
	/*UPROPERTY()
	GameSate*/
	UPROPERTY()
	UPlayerProgressionSubsystem* PlayerProgression;
	UPROPERTY()
	UWorldStateSubsystem* WorldState;
	
	UPROPERTY()
	APlayerHUD* HUD;
	
	//Visual Effects
	UPROPERTY()
	UNiagaraComponent* SpeedEffect;
	
	//Exit the tutorial area
	void StartGame(EGamePhase GameState);
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
	virtual void UpdateHudStats() override;
	void UpdateCurrentSpeed();
	void CalculateDamage();
	virtual void ShotFired() override;
	void CheckPlayerHeath();
	

protected:
	FTimerHandle UpdateCurrentSpeed_TimeHandle;
	float CurrentSpeed;
	float DamageScaling = 0.1f;
	FPlayerStats PlayerStats;
	float Health = 100.0f;
	
};
