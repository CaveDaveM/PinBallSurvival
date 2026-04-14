// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

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
	
	APlayerHUD();
	void ToggleInGameMenu();
	void HideInGameMenu();
	void DisplayInGameMenu();
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UStaticWindow* StaticMenuWidget;
	
	bool bIsMenuVisible = false;
};
