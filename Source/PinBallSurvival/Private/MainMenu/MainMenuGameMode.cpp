// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenuGameMode.h"
#include "MainMenu/MainMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/SaveGameSubsytem.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	//Dont spawn in an actual player character, dont need him
	DefaultPawnClass = nullptr;
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("No player controller found"));
	}
	
	/*USaveGameSubsytem* SavedGameSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsytem>();
	if (SavedGameSystem)
	{
		SavedGameSystem->LoadGame();
	}*/
	
	
	TArray<AActor*> CameraFound;
	UGameplayStatics::GetAllActorsOfClassWithTag(
		this,
		ACameraActor::StaticClass(), 
		MenuCameraTag,
		CameraFound);
	
	if (CameraFound.Num() > 0)
	{
		PC->SetViewTargetWithBlend(CameraFound[0]);
		GEngine->AddOnScreenDebugMessage(-1,10.0f, FColor::Yellow, TEXT("CameraFound"));
	}
	
	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UMainMenuWidget>(PC, MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->LoadPlayerProgression();
			MainMenuWidget->AddToViewport();
		}
	}
	
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
}
