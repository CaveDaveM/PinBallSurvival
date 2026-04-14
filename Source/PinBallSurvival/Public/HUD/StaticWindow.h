// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerProgressionSubsystem.h"
#include "StaticWindow.generated.h"

class UPlayerProgressionSubsystem;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API UStaticWindow : public UUserWidget
{
	GENERATED_BODY()


public:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentLevel;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* XPDisplay;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Speed;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ProjectileDamage;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHealth;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AvailableLevelUpText;
	
	UPROPERTY(meta = (BindWidget))
	UButton* UpgradeHealth;
	
	UPROPERTY(meta = (BindWidget))
	UButton* UpgradeSpeed;
	
	UPROPERTY(meta = (BindWidget))
	UButton* UpgradeDamage;
	
	UPROPERTY()
	UPlayerProgressionSubsystem* PlayerProgression;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnPlayerStatsChange(FPlayerStats NewPlayerStats);
	
	UFUNCTION()
	void UpdatePlayerStatistics();
	UFUNCTION()
	void ChoiceHealth();
	UFUNCTION()
	void ChoiceSpeed();
	UFUNCTION()
	void ChoiceDamage();
	UFUNCTION()
	void ApplyLevelUp(int32 NewLevel);
	
	int32 AvailableLevelUps = 0;
	int32 PlayerLevel = 1;
	
	FPlayerStats PlayerStats;	
};
