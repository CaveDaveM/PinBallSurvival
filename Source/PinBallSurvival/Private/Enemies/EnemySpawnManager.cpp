// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemySpawnManager.h"

#include "MyPawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawnManager::AEnemySpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	EnemySpawnBounds = CreateDefaultSubobject<UBoxComponent>("EnemySpawnBounds");
	EnemySpawnBounds->SetBoundsScale(40.0f);
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
    		StartWave();
    	}
    	else
    	{
    		UE_LOG(LogTemp, Error, TEXT("WaveSpawnTable is NULL"));
    	}

	
	
	/*//TEST STRUCT
	TArray<int32> TestEnemyArray;
	for (int32 i = 0; i < 16; ++i)
	{
		TestEnemyArray.Add(i);
	}
	EnemySpawnExampleTable.EnemiesToSpawn = TestEnemyArray;
	
	DebugTestLocations();*/
	
	StartWave();
}

TArray<FVector> AEnemySpawnManager::FindSpawnOffsets(float radius)
{
	const float AngleStep = 360.0f / CurrentWaveData->Amount;
	float CurrentAngle = 0.0f;
	TArray<FVector> EnemiesSpawnOffset;
	for (int i = 0; i < CurrentWaveData->Amount; ++i)
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
	TArray<FVector> EnemiesSpawnOffset = FindSpawnOffsets(500.0f);
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
		StartWaveSelection();
	}
}

void AEnemySpawnManager::StartWaveSelection()
{
	EnemyCount = 0;
	bSpawningComplete = false;
	EnemySpawnOffsets = FindSpawnOffsets(500.0f); 
	GetWorld()->GetTimerManager().SetTimer(
		WaveSpawner_TimeHandler,
		this,
		&AEnemySpawnManager::SpawnEnemies,
		CurrentWaveData->SpawnDelay,
		true);
}

void AEnemySpawnManager::SpawnEnemies()
{
	if (EnemyCount > CurrentWaveData->Amount) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	FVector PlayerLocation = PlayerReference->GetActorLocation();
	FVector SpawnLocation =  PlayerLocation + EnemySpawnOffsets[EnemyCount];
	
	FRotator SpawnRotation = FRotator::ZeroRotator;

	AMyPawn* Enemy = GetWorld()->SpawnActor<AMyPawn>(
		CurrentWaveData->Enemy,
		SpawnLocation,
		SpawnRotation,
		SpawnParams);
	
	
	
	//DrawDebugSphere(GetWorld(), SpawnLocation, 50.0f, 12, FColor::Red, true, 250.0f);
	if (Enemy)
	{
		Enemy->SpawnDefaultController();
	}

	EnemyCount++;

	if (EnemyCount == CurrentWaveData->Amount)
	{
		GetWorldTimerManager().ClearTimer(WaveSpawner_TimeHandler);
		EndWave();
	}
	// this way of doing the spawn does allow for 
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
			CurrentWaveData->SectionSpawnDelay,
			false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Waves Ended"));
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Red,TEXT("Game Ended"));
	}
}

// Called every frame
void AEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

