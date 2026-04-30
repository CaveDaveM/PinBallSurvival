// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Settings = GEngine->GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogTemp, Error, TEXT("Could Not Find User Settings"));
	}
	
	if (ChooseMapOne)
	{
		ChooseMapOne->OnClicked.AddDynamic(this, &UMainMenuWidget::OnMapOneButtonPressed);
	}
	if (ChooseMapTwo)
	{
		ChooseMapTwo->OnClicked.AddDynamic(this, &UMainMenuWidget::OnMapTwoButtonPressed);
	}
	if (ResolutionBox)
	{
		ResolutionBox->OnSelectionChanged.AddDynamic(this, &UMainMenuWidget::OnResolutionChanged);
	}
	if (FullscreenCheckBox)
	{
		FullscreenCheckBox->OnCheckStateChanged.AddDynamic(this, &UMainMenuWidget::OnFullscreenChanged);
	}
	if (MasterVolumeSlider)
	{
		MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UMainMenuWidget::OnMasterVolumeChanged);
	}
	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnApplyButtonPressed);
	}
	
	PopulateResoultions();
	LoadCurrentSettings();
}

void UMainMenuWidget::OnMapOneButtonPressed()
{
	FString ChosenMapName = FString("TesterLvl");
	FString MapName = MapLocationText + ChosenMapName;
	GetWorld()->SeamlessTravel(MapName);
}

void UMainMenuWidget::OnMapTwoButtonPressed()
{
	FString ChosenMapName = FString("TesterLvl1");
	FString MapName = MapLocationText + ChosenMapName;
	GetWorld()->SeamlessTravel(MapName);
}

void UMainMenuWidget::PopulateResoultions()
{
	ResolutionBox->ClearOptions();
	
	TArray<FIntPoint> Resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
	
	for (const FIntPoint& Resolution : Resolutions)
	{
		ResolutionBox->AddOption(FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y));
	}
}

void UMainMenuWidget::LoadCurrentSettings()
{
	FIntPoint CurrentResolution = Settings->GetScreenResolution();
	if (ResolutionBox)
	{
		ResolutionBox->SetSelectedOption(FString::Printf(TEXT("%dx%d"),CurrentResolution.X,CurrentResolution.Y));
	}
	if (FullscreenCheckBox)
	{
		FullscreenCheckBox->SetIsChecked(Settings->GetFullscreenMode() == EWindowMode::Fullscreen);
	}
	
}


void UMainMenuWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FString LeftRes, RightRes;
	if (!SelectedItem.Split(TEXT("x"), &LeftRes, &RightRes)) return;
	
	FIntPoint NewRes(FCString::Atoi(*LeftRes), FCString::Atoi(*RightRes));
	{
		Settings->SetScreenResolution(NewRes);
	}
}

void UMainMenuWidget::OnFullscreenChanged(bool bIsChecked)
{
	if (Settings)
	{
		Settings->SetFullscreenMode(bIsChecked ? EWindowMode::Fullscreen : EWindowMode::Windowed);
	}
}

void UMainMenuWidget::OnMasterVolumeChanged(float Value)
{
	//Update if i actually make a sound subsystem
}

void UMainMenuWidget::OnApplyButtonPressed()
{
	if (Settings)
	{
		Settings->ApplySettings(false);
		Settings->SaveSettings();
	}
}


