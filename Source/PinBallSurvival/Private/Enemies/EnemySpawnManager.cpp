// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemySpawnManager.h"

#include "MyPawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AEnemySpawnManager::AEnemySpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void AEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	/** NOTE: i do not like this implementation, its static and doesnt allow for a change of spawner shape.
	 * TODO: Find a way to make this a dynamic spawn location **/
	
	PlayerReference = UGameplayStatics::GetPlayerPawn(this, 0);
	
	if (PlayerReference)
    {
    	UE_LOG(LogTemp, Display, TEXT("WaveManager, Cast to PlayerCharacter, Success"));
    }
    else
    {
    	UE_LOG(LogTemp, Error, TEXT("WaveManager, Cast to PlayerCharacter, Failure"));
    }
    if (WaveSpawnTable)
    {
    	UE_LOG(LogTemp, Display, TEXT("WaveSpawnTable already exist"));
    }
    else
    {
    	UE_LOG(LogTemp, Error, TEXT("WaveSpawnTable is NULL"));
    }
	//Subscribers
	PlayerProgression = GetWorld()->GetSubsystem<UPlayerProgressionSubsystem>();
	if (!PlayerProgression)
	{
		UE_LOG(LogLevel, Warning, TEXT("Player Progression not found in EnemySpawnManager.CPP"));
	}
	
	WorldState = GetWorld()->GetSubsystem<UWorldStateSubsystem>();
	if (WorldState)
	{
		WorldState->OnGameInProgress.AddUObject(this, &AEnemySpawnManager::StartGame);
	}
	else
	{
		UE_LOG(LogLevel, Warning, TEXT("World State not found in EnemySpawnManager.CPP"));
	}
	
	/*//TEST STRUCT
	TArray<int32> TestEnemyArray;
	for (int32 i = 0; i < 16; ++i)
	{
		TestEnemyArray.Add(i);
	}
	EnemySpawnExampleTable.EnemiesToSpawn = TestEnemyArray;
	
	DebugTestLocations();*/
}

void AEnemySpawnManager::StartGame(EGamePhase GameState)
{
	// might be a redundant check, but all the bugs ive seen, im not taking any chances
	if (GameState == EGamePhase::Playing)
	{
		StartWave();
	}
}


TArray<FVector> AEnemySpawnManager::FindSpawnOffsets(float radius)
{
	const float AngleStep = 360.0f / CurrentWaveData->Sections[SectionInt].Amount;
	float CurrentAngle = 0.0f;
	TArray<FVector> EnemiesSpawnOffset;
	for (int i = 0; i < CurrentWaveData->Sections[SectionInt].Amount; ++i)
	{
		float Radians = FMath::DegreesToRadians(CurrentAngle);
		FVector SpawnOffset(
			FMath::Cos(Radians) * radius,
			FMath::Sin(Radians) * radius,
			0.0f);
		
		EnemiesSpawnOffset.Add(SpawnOffset);
		CurrentAngle += AngleStep;
	}
	
	return EnemiesSpawnOffset;
}

void AEnemySpawnManager::DebugTestLocations()
{
	FVector PlayerLocation = PlayerReference->GetActorLocation();
	TArray<FVector> EnemiesSpawnOffset = FindSpawnOffsets(5000.0f);
	for (int i = 0; i < EnemiesSpawnOffset.Num(); ++i)
	{
		FVector EnemySpawnLocation = PlayerLocation + EnemiesSpawnOffset[i];
		DrawDebugSphere(GetWorld(),EnemySpawnLocation,25.0f,30,FColor::Red,true);
	}
}
void AEnemySpawnManager::StartWave()
{
	CurrentWaveData = WaveSpawnTable->FindRow<FWaveSpawnParams>(GetCurrentWaveName(),"");
	if (CurrentWaveData)
	{
		UE_LOG(LogTemp, Display, TEXT("Wave Started"));
		SectionInt = 0;
		StartWaveSelection();
	}
}
void AEnemySpawnManager::StartWaveSelection()
{
	EnemyCount = 0;
	bSpawningComplete = false;
	EnemySpawnOffsets = FindSpawnOffsets(1000.0f); 
	GetWorld()->GetTimerManager().SetTimer(
		WaveSpawner_TimeHandler,
		this,
		&AEnemySpawnManager::SpawnEnemies,
		CurrentWaveData->Sections[SectionInt].SpawnDelay,
		true);
}

void AEnemySpawnManager::SpawnEnemies()
{
	if (EnemyCount > CurrentWaveData->Sections[SectionInt].Amount) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	FVector PlayerLocation = PlayerReference->GetActorLocation();
	FVector SpawnLocation =  PlayerLocation + EnemySpawnOffsets[EnemyCount];
	
	FRotator SpawnRotation = FRotator::ZeroRotator;

	AMyPawn* Enemy = GetWorld()->SpawnActor<AMyPawn>(
		CurrentWaveData->Sections[SectionInt].Enemy,
		SpawnLocation,
		SpawnRotation,
		SpawnParams);
	
	if (Enemy)
	{
		Enemy->SpawnDefaultController();
		Enemy->OnDestroyed.AddDynamic(this, &AEnemySpawnManager::OnEnemyKilled);
	}

	EnemyCount++;

	if (EnemyCount == CurrentWaveData->Sections[SectionInt].Amount)
	{
		GetWorldTimerManager().ClearTimer(WaveSpawner_TimeHandler);
		EndWaveSection();
	}
	// this way of doing the spawn does allow for 
}

void AEnemySpawnManager::EndWaveSection()
{
	SectionInt++;
	
	if (CurrentWaveData->Sections.Num() > SectionInt)
	{
		GetWorldTimerManager().SetTimer(
			WaveSpawner_TimeHandler,
			this,
			&AEnemySpawnManager::StartWaveSelection,
			CurrentWaveData->Sections[SectionInt].EndSectionInterval,
			false);
	}
	else if (CurrentWaveData->Sections.Num() <= SectionInt)
	{
		EndWave();
	}
}

void AEnemySpawnManager::EndWave()
{
	CurrentWaveInt++;
	CurrentWaveData = WaveSpawnTable->FindRow<FWaveSpawnParams>(GetCurrentWaveName(),"");
	if (CurrentWaveData)
	{
		GetWorldTimerManager().SetTimer(
			WaveSpawner_TimeHandler,
			this,&AEnemySpawnManager::StartWave,
			CurrentWaveData->EndWaveIntervals,
			false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Waves Ended"));
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Red,TEXT("Game Ended"));
		GetWorld()->GetTimerManager().ClearTimer(WaveSpawner_TimeHandler);
		GetWorld()->GetTimerManager().SetTimer(
			WaveSpawner_TimeHandler,
			this,
			&AEnemySpawnManager::EndGame,
			20.0f,
			false);
	}
}

void AEnemySpawnManager::EndGame()
{
	APinballGameState* PinballGS = GetWorld()->GetGameState<APinballGameState>();
	if (PinballGS)
	{
		PinballGS->SetGamePhase(EGamePhase::Ended, true);
	}
}

void AEnemySpawnManager::OnEnemyKilled(AActor* Enemy)
{
	Enemy->OnDestroyed.RemoveDynamic(this, &AEnemySpawnManager::OnEnemyKilled);
	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		DeathEffects,
		Enemy->GetActorLocation());
	PlayerProgression->AddXP(1);
}

// Called every frame
void AEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

