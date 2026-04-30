// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

class UMainMenuWidget;
/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMainMenuGameMode();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(editDefaultsOnly)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	FName MenuCameraTag = TEXT("MenuCamera");
	
	UPROPERTY(editDefaultsOnly)
	UMainMenuWidget* MainMenuWidget;
};
