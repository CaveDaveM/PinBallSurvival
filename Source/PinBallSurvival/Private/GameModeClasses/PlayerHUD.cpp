// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeClasses/PlayerHUD.h"
#include "HUD/EndOfGame/EndGameWidget.h"
#include "HUD/TutorialWidget.h"
#include "HUD/InteractionWidget.h"
#include "HUD/StaticWindow.h"
#include "Kismet/GameplayStatics.h"

APlayerHUD::APlayerHUD()
{
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	WorldState = GetWorld()->GetSubsystem<UWorldStateSubsystem>();
	if (WorldState)
	{
		WorldState->OnGameStart.AddUObject(this, &APlayerHUD::ToggleStartGameMenu);
		WorldState->OnGameEnded.AddUObject(this, &APlayerHUD::ShowEndGameMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WorldState in playerhub cant be found"));
	}
	
	if (StaticWindowClass)
	{
		StaticMenuWidget = CreateWidget<UStaticWindow>(GetWorld(),StaticWindowClass);
		StaticMenuWidget->AddToViewport(5);
		StaticMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (TutorialWidgetClass)
	{
		TutorialWidget = CreateWidget<UTutorialWidget>(GetWorld(), TutorialWidgetClass);
		TutorialWidget->AddToViewport(5);
		TutorialWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::ToggleInGameMenu()
{
	if(bIsMenuVisible)
	{
		HideInGameMenu();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1.0);
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
	}
	else
	{
		DisplayInGameMenu();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.1);
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
	}
}

void APlayerHUD::HideInGameMenu()
{
	if (StaticMenuWidget)
	{
		bIsMenuVisible = false;
		StaticMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::DisplayInGameMenu()
{
	if (StaticMenuWidget)
	{
		bIsMenuVisible = true;
		StaticMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
// Tutorial Section
void APlayerHUD::ToggleStartGameMenu(EGamePhase GamePhase) const
{

	if (GamePhase == EGamePhase::StartPlay)
	{
		DisplayStartGameMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
	}
	else if (GamePhase == EGamePhase::Playing)
	{
		HideStartGameMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
	}
}
void APlayerHUD::DisplayStartGameMenu() const
{
	if (TutorialWidget)
	{
		TutorialWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
void APlayerHUD::HideStartGameMenu() const
{
	if (TutorialWidget)
	{
		TutorialWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
//EndOfGame
void APlayerHUD::ShowEndGameMenu(EGamePhase GamePhase, bool bIsGameWon)
{
	if (GamePhase != EGamePhase::Ended)
	{
		return;
	}
	
	if (EndOfGameMenuClass)
	{
		const FInputModeUIOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		EndGameWidget = CreateWidget<UEndGameWidget>(GetWorld(), EndOfGameMenuClass);
		EndGameWidget->OnGameFinished(bIsGameWon);
		EndGameWidget->AddToViewport(6);
		EndGameWidget->SetVisibility(ESlateVisibility::Visible);
	}
}



