// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "AlwaysOnDisplay.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API UAlwaysOnDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;
	
	void UpdateHealthBar(float CurrentHealth, float MaxHealth) const;
	void UpdateAmmoText(int32 CurrentAmmo, int32 MaxAmmo) const;
};
