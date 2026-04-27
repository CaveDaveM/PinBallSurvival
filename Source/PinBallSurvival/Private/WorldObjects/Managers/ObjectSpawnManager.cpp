// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/Managers/ObjectSpawnManager.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Subsystems/WorldStateSubsystem.h"
#include "WorldObjects/BaseWorldObject.h"

// Sets default values
AObjectSpawnManager::AObjectSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}
//Setters and getters
void AObjectSpawnManager::SetNewRarityTable(const TArray<FRarityWeights>& NewRarityTable)
{
	RarityTable.Empty();
	RarityTable = NewRarityTable;
}
// Called when the game starts or when spawned
void AObjectSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	SortWorldObjects();
	
	WorldState = GetWorld()->GetSubsystem<UWorldStateSubsystem>();
	if (WorldState)
	{
		WorldState->OnGameInProgress.AddUObject(this, &AObjectSpawnManager::StartSpawingObjects);
		UE_LOG(LogLevel, Log, TEXT("WorldState, object manager"));
	}
	else
	{
		UE_LOG(LogLevel, Warning, TEXT("Couldnt Find WorldState, Object Manager"));
	}
}

void AObjectSpawnManager::StartSpawingObjects(EGamePhase GameState)
{
	if (GameState == EGamePhase::Playing)
	{
		GetWorld()->GetTimerManager().SetTimer(
			SpawnWorldObjects_TimerHandle,
			this,
			&AObjectSpawnManager::FindWorldObjectsArray,
			10.0f,
			true);
	}
}

void AObjectSpawnManager::SortWorldObjects()
{
	for (FWorldObjectData WorldObject : WorldObjectData)
	{
		switch (WorldObject.Rarity) 
		{
			case EObjectRarity::Common:
				{
					CommonObjects.Add(WorldObject);
					break;
				}
			case EObjectRarity::Rare:
				{
					RareObjects.Add(WorldObject);
					break;
				}
			case EObjectRarity::Unique:
				{
					UniqueObjects.Add(WorldObject);
					break;
				}
			default:
				{
					UE_LOG(LogTemp, Warning, TEXT("Missing Rarity in World Object Data"));
				}
		}
	}
}


EObjectRarity AObjectSpawnManager::FindRarity()
{
	// using a weighted table to ensure rarity when spawning world objects, 
	// got from https://copyprogramming.com/howto/how-to-make-a-function-that-gets-me-3-diferent-ramdom-numbers
	float TotalWeight = 0.0f;
	for (FRarityWeights WorldObject : RarityTable)
	{
		TotalWeight += WorldObject.Weight;
	}

	float RandomWeight = FMath::FRandRange(0.f, TotalWeight);

	// moves through the rarity to see where the rarity lands.
	float CumulatedWeight = 0.0f;
	for (FRarityWeights WorldObject : RarityTable)
	{
		CumulatedWeight += WorldObject.Weight;
		if (RandomWeight <= CumulatedWeight)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, *UEnum::GetValueAsString(WorldObject.Rarity));
			return WorldObject.Rarity;
		}
	}
	return RarityTable[1].Rarity;
}

void AObjectSpawnManager::FindWorldObjectsArray()
{
	EObjectRarity Rarity = FindRarity();
	switch (Rarity)
	{
	case EObjectRarity::Common:
		{
			SpawnWorldObjects(CommonObjects);
			break;
		}
	case EObjectRarity::Rare:
		{
			SpawnWorldObjects(RareObjects);
			break;
		}
	case EObjectRarity::Unique:
		{
			SpawnWorldObjects(UniqueObjects);
			break;
		}
	default: ;
	}
}

FVector3d AObjectSpawnManager::FindSpawnLocation()
{
	//Change this to bounds of a cube for easy adjustments
	const float RandomXCord = FMath::FRandRange(-5400.0f, 10600);
	const float RandomYCord = FMath::FRandRange(-13750.0f, 10500.0f);
	const FVector3d WorldPosition = FVector3d(RandomXCord, RandomYCord, 40.0f);
	
	return WorldPosition;
}
void AObjectSpawnManager::SpawnWorldObjects( const TArray<FWorldObjectData>& WorldObjects)
{
	if (WorldObjects.IsEmpty()) return;
	
	int32 RandomObject = FMath::RandRange(0, WorldObjects.Num() - 1);
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	FVector SpawnLocation = FindSpawnLocation();
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation);
	
	ABaseWorldObject* SpawnedObject = GetWorld()->SpawnActor<ABaseWorldObject>(
		WorldObjects[RandomObject].BaseWorldObject,
		SpawnTransform,
		SpawnParameters);
	if (SpawnedObject)
	{
		SpawnedObject->ObjectType = WorldObjects[RandomObject].ObjectType;
		SpawnedObject->SetObjectRarity(WorldObjects[RandomObject].Rarity);
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			SpawnIndication,
			SpawnedObject->GetActorLocation());
		if (NiagaraComp)
		{
			NiagaraComp->SetRelativeScale3D(FVector(3.0f, 3.0f, 1.0f));
		}
		SpawnedObject->OnDestroyed.AddDynamic(this, &AObjectSpawnManager::OnObjectCollected);
		WorldState->RegisterWorldObject(SpawnedObject);
	}

}

void AObjectSpawnManager::OnObjectCollected(AActor* CollectedObjects)
{
	CollectedObjects->OnDestroyed.RemoveDynamic(this, &AObjectSpawnManager::OnObjectCollected);
	ABaseWorldObject* CollectedObject = Cast<ABaseWorldObject>(CollectedObjects);
	if (CollectedObject)
	{
		WorldState->UnregisterWorldObject(CollectedObject);
	}
}

// Called every frame
void AObjectSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


