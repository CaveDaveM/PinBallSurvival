// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeClasses/PlayerHUD.h"

#include "HUD/InteractionWidget.h"
#include "HUD/StaticWindow.h"

APlayerHUD::APlayerHUD()
{
	
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	if (StaticWindowClass)
	{
		StaticMenuWidget = CreateWidget<UStaticWindow>(GetWorld(),StaticWindowClass);
		StaticMenuWidget->AddToViewport(5);
		StaticMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::ToggleInGameMenu()
{
	if(bIsMenuVisible)
	{
		HideInGameMenu();
		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()-> SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayInGameMenu();
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

