// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class USlider;
class UCheckBox;
class UComboBoxString;
class UButton;
/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//Play Game Map Choices
	UPROPERTY(meta =(BindWidget))
	UButton* ChooseMapOne;
	
	UPROPERTY(meta =(BindWidget))
	UButton* ChooseMapTwo;

	//OptionsMenu
	UPROPERTY(meta =(BindWidget))
	UComboBoxString* ResolutionBox;
	
	UPROPERTY(meta =(BindWidget))
	UCheckBox* FullscreenCheckBox;
	
	UPROPERTY(meta =(BindWidget))
	USlider* MasterVolumeSlider;
	
	UPROPERTY(meta =(BindWidget))
	UButton* ApplyButton;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	//Play Game Map Choices
	UFUNCTION()
	void OnMapOneButtonPressed();
	UFUNCTION()
	void OnMapTwoButtonPressed();
	
	FString MapLocationText = FString("/Game/Maps/");
	
	//OptionsMenu
	UFUNCTION()
	void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnFullscreenChanged(bool bIsChecked);
	UFUNCTION()
	void OnMasterVolumeChanged(float Value);
	UFUNCTION()
	void OnApplyButtonPressed();
	
	void PopulateResoultions();
	void LoadCurrentSettings();
	
private:
	UPROPERTY()
	UGameUserSettings* Settings;
};
