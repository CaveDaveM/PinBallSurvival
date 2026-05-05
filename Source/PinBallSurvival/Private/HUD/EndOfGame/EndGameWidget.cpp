// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EndOfGame/EndGameWidget.h"

#include "PlayerProgressionSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Subsystems/WorldStateSubsystem.h"

void UEndGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UEndGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (GameStatsWindow)
	{
		
	}
	if (ReturnToMenuButton)
	{
		ReturnToMenuButton->OnClicked.AddDynamic(this, &UEndGameWidget::OnReturnToMenuButtonClicked);
	}
}

void UEndGameWidget::OnReturnToMenuButtonClicked()
{
	GetWorld()->SeamlessTravel("/Game/Maps/MainMenu.MainMenu");
}

void UEndGameWidget::DisplayGameStatsWindow(bool bIsGameWon, int32 Xp)
{
	if (bIsGameWon)
	{
		GameStatsWindow->SetText(FText::Format(FText::FromString(" YOU HAVE WON!!. You Have Gained {0} XP"),
			Xp));
	}
	else
	{
		GameStatsWindow->SetText(FText::Format(FText::FromString(" YOU HAVE LOST. You Have Gained {0} XP"),
			Xp));
	}
}

void UEndGameWidget::OnGameFinished(bool bIsGameWon)
{
	PlayerProgression = GetWorld()->GetSubsystem<UPlayerProgressionSubsystem>();
	if (PlayerProgression)
	{
		//UE_LOG(LogTemp, Error, TEXT("PlayerProgressionSubsystem in endgamewidget is NULL"));
		int32 GainedXp = PlayerProgression->CalculateEndGameXP(bIsGameWon);
		if (bIsGameWon)
		{
			DisplayGameStatsWindow(bIsGameWon, GainedXp);
		}
		else
		{
			DisplayGameStatsWindow(bIsGameWon, GainedXp);
		}
	}
}
