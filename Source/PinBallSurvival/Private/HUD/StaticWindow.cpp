// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/StaticWindow.h"

#include "PinballCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UStaticWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UGameInstance* GI = GetGameInstance();
	if (GI)
	{
		PlayerProgression = GI->GetSubsystem<UPlayerProgressionSubsystem>();
		if (PlayerProgression)
		{
			UE_LOG(LogLevel, Log, TEXT("Player Progression Enabled"));
			PlayerProgression->OnPlayerStats.AddDynamic(this, &UStaticWindow::OnPlayerStatsChange);
		}
		else
		{
			UE_LOG(LogLevel, Warning, TEXT("Player Progression not found in StaticWindow.cpp"));
		}
	}
}

void UStaticWindow::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UpgradeHealth)
	{
		UpgradeHealth->OnClicked.AddDynamic(this, &UStaticWindow::ChoiceHealth);
	}
	if (UpgradeSpeed)
	{
		UpgradeSpeed->OnClicked.AddDynamic(this, &UStaticWindow::ChoiceSpeed);
	}
	if (UpgradeDamage)
	{
		UpgradeDamage->OnClicked.AddDynamic(this, &UStaticWindow::ChoiceDamage);
	}
	if (PlayerProgression)
	{
		PlayerProgression->OnLevelUp.AddDynamic(this, &UStaticWindow::ApplyLevelUp);
	}

}

void UStaticWindow::OnPlayerStatsChange(FPlayerStats NewPlayerStats)
{
	PlayerStats = NewPlayerStats;
	UpdatePlayerStatistics();
}

void UStaticWindow::UpdatePlayerStatistics()
{
	CurrentLevel->SetText(FText::Format(FText::FromString("Player Level {0}: {1}/{2}"),
		PlayerLevel,
		PlayerProgression->CalculateNextLevelXPRequirement(),
		PlayerProgression->GetCurrentXP()));
	
	XPDisplay->SetText(FText::Format(FText::FromString("XP : {0}/{1}"),
		PlayerProgression->CalculateNextLevelXPRequirement(),
		PlayerProgression->GetCurrentXP()));
	
	MaxHealth->SetText(FText::Format(FText::FromString("MAX HP : {0}"),
		PlayerStats.MaxHealth));
		
}

void UStaticWindow::ChoiceHealth()
{
	if (AvailableLevelUps > 0)
	{
		PlayerProgression->SetUpgradeValues(EUpgrades::MaxHealth);
	}
	AvailableLevelUps -= 1;
}

void UStaticWindow::ChoiceSpeed()
{
	if (AvailableLevelUps > 0)
	{
		PlayerProgression->SetUpgradeValues(EUpgrades::MoveSpeed);
	}
	AvailableLevelUps -= 1;
}

void UStaticWindow::ChoiceDamage()
{
	if (AvailableLevelUps > 0)
	{
		PlayerProgression->SetUpgradeValues(EUpgrades::ProjectileDamage);
	}
	AvailableLevelUps -= 1;
}
// makes sure that if the player levels up more than once, he still gets to keep the old level
void UStaticWindow::ApplyLevelUp(int32 NewLevel)
{
	AvailableLevelUps++;
	UpdatePlayerStatistics();
}
