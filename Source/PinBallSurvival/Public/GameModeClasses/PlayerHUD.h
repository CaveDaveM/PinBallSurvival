// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Subsystems/WorldStateSubsystem.h"
#include "PlayerHUD.generated.h"

class UEndGameWidget;
class UTutorialWidget;
class UStaticWindow;
/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category = "Hud")
	TSubclassOf<UStaticWindow> StaticWindowClass;
	
	UPROPERTY(EditDefaultsOnly,Category = "Hud")
	TSubclassOf<UTutorialWidget> TutorialWidgetClass;
	
	UPROPERTY(EditDefaultsOnly,Category = "Hud")
	TSubclassOf<UEndGameWidget> EndOfGameMenuClass;
	
	APlayerHUD();
	void ToggleInGameMenu();
	void HideInGameMenu();
	void DisplayInGameMenu();
	
	//Hide the tutorial menu
	void HideStartGameMenu() const;
	void DisplayStartGameMenu() const;
	

protected:
	
	UPROPERTY()
	UWorldStateSubsystem* WorldState;
	UPROPERTY()
	UStaticWindow* StaticMenuWidget;
	UPROPERTY()
	UTutorialWidget* TutorialWidget;
	UPROPERTY()
	UEndGameWidget* EndGameWidget;
	
	bool bIsMenuVisible = false;
	
	virtual void BeginPlay() override;
	void ToggleStartGameMenu(EGamePhase GamePhase) const;
	void ShowEndGameMenu(EGamePhase GamePhase, bool bIsGameWon);
};
