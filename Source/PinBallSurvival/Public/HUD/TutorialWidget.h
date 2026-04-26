// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

class APinballGameState;
class UButton;
/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnStart();
	
	UPROPERTY()
	APinballGameState* PinballGS;
};
