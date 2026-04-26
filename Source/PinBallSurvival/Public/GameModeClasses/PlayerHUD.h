// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Subsystems/WorldStateSubsystem.h"
#include "PlayerHUD.generated.h"

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
	
	APlayerHUD();
	void ToggleInGameMenu();
	void HideInGameMenu();
	void DisplayInGameMenu();
	
	//Hide the tutorial menu 
	UFUNCTION()
	void ToggleStartGameMenu(EGamePhase GamePhase) const;
	void HideStartGameMenu() const;
	void DisplayStartGameMenu() const;
	

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UWorldStateSubsystem* WorldState;

	UPROPERTY()
	UStaticWindow* StaticMenuWidget;
	
	UPROPERTY()
	UTutorialWidget* TutorialWidget;
	
	bool bIsMenuVisible = false;
};
