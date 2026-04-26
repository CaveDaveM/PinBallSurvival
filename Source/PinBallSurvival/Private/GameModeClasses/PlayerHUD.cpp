// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeClasses/PlayerHUD.h"
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
		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()-> SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayInGameMenu();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.1);
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()-> SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
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
		GEngine->AddOnScreenDebugMessage(-1,100.0f,FColor::Green,TEXT("Toggle Start Game Menu"));
		DisplayStartGameMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()-> SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
	else if (GamePhase == EGamePhase::Playing)
	{
		HideStartGameMenu();
		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()-> SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
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


