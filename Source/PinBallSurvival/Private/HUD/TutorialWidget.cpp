// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/TutorialWidget.h"

#include "Components/Button.h"
#include "GameModeClasses/PinballGameState.h"

void UTutorialWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (const UWorld* World = GetWorld())
	{
		PinballGS = World->GetGameState<APinballGameState>();
		if (!PinballGS)
		{
			UE_LOG(LogTemp, Error, TEXT("PinballGS is NULL in TutorialWidget"));
		}
	}
}

void UTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UTutorialWidget::OnStart);
	}
}

void UTutorialWidget::OnStart()
{
	PinballGS->SetGamePhase(EGamePhase::Playing);
}
