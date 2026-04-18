// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/AlwaysOnDisplay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UAlwaysOnDisplay::UpdateHealthBar(float CurrentHealth, float MaxHealth) const
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

void UAlwaysOnDisplay::UpdateAmmoText(int32 CurrentAmmo, int32 MaxAmmo) const
{
	if (AmmoText)
	{
		AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),CurrentAmmo,MaxAmmo)));
	}
}
