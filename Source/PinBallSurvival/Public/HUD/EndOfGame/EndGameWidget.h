// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameModeClasses/PinballGameState.h"
#include "EndGameWidget.generated.h"

class UWorldStateSubsystem;
class UPlayerProgressionSubsystem;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API UEndGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameStatsWindow;	
	
	UPROPERTY(meta = (BindWidget))
	UButton* ReturnToMenuButton;
	
	void OnGameFinished(bool bIsGameWon);
protected:
	UPROPERTY()
	UWorldStateSubsystem* WorldState;
	UPROPERTY()
	UGameInstanceSubsystem* SaveGameSubsystem;
	UPROPERTY()
	UPlayerProgressionSubsystem* PlayerProgression;
	
	UFUNCTION()
	void OnReturnToMenuButtonClicked();
	void DisplayGameStatsWindow(bool bIsGameWon, int32 Xp);
};
