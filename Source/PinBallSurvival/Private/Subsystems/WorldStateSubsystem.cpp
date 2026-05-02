// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/WorldStateSubsystem.h"

#include "WorldObjects/BaseWorldObject.h"
#include "WorldObjects/Managers/WorldObjectData.h"

void UWorldStateSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	if (APinballGameState* PinballGS = InWorld.GetGameState<APinballGameState>())
	{
		PinballGS->OnStateChange.AddUObject(this, &UWorldStateSubsystem::SwitchOnGameStateChange);
	}
}

void UWorldStateSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UWorldStateSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UWorldStateSubsystem::SwitchOnGameStateChange(EGamePhase ChangedState)
{
	switch (ChangedState) {
	case None:
		{
			break;
		}
	case StartPlay:
		{
			OnGameStart.Broadcast(StartPlay);
			break;
		}
	case Playing:
		{
			GEngine->AddOnScreenDebugMessage(-1,100.0f,FColor::Green,"WorldStateGame Playing");
			OnGameStart.Broadcast(Playing);
			OnGameInProgress.Broadcast(Playing);
			break;
		}
	case Ended:
		{
			OnGameEnded.Broadcast(Ended);
			break;
		}
	}

}


void UWorldStateSubsystem::RegisterWorldObject(ABaseWorldObject* SpawnedObject)
{
	switch (SpawnedObject->ObjectType) 
	{
		case EObjectType::HealingObject:
			{
				RegisteredHealingObjects.Add(SpawnedObject);
				break;
			}
		case EObjectType::AmmoObject:
			{
				RegisteredAmmoObjects.Add(SpawnedObject);
				break;
			}
		case EObjectType::PlayerUpgrades:
			{
				RegisteredUpgradeObjects.Add(SpawnedObject);
				break;
			}
		default:
			{
				UE_LOG(LogTemp, Error, TEXT("SpawnedObject Registered incorrectly"));
			}
	}
	
}

void UWorldStateSubsystem::UnregisterWorldObject(ABaseWorldObject* SpawnedObject)
{
	switch (SpawnedObject->ObjectType) 
	{
	case EObjectType::HealingObject:
		{
			RegisteredHealingObjects.Remove(SpawnedObject);
			break;
		}
	case EObjectType::AmmoObject:
		{
			RegisteredAmmoObjects.Remove(SpawnedObject);
			break;
		}
	case EObjectType::PlayerUpgrades:
		{
			RegisteredUpgradeObjects.Remove(SpawnedObject);
			break;
		}
	default:
		{
			UE_LOG(LogTemp, Error, TEXT("SpawnedObject deRegistered incorrectly"));
		}
	}}

ABaseWorldObject* UWorldStateSubsystem::GetClosestWorldObjectByType(EObjectType Type, FVector ActorLocation)
{
	switch (Type) {
	case EObjectType::HealingObject:
		{
			return FindClosestObjectFromLocation(RegisteredHealingObjects, ActorLocation);
		}
	case EObjectType::AmmoObject:
		{
			return FindClosestObjectFromLocation(RegisteredAmmoObjects, ActorLocation);
		}
	case EObjectType::PlayerUpgrades:
		{
			return FindClosestObjectFromLocation(RegisteredUpgradeObjects, ActorLocation);
		}
	}
	
	return nullptr;
}

ABaseWorldObject* UWorldStateSubsystem::FindClosestObjectFromLocation(TArray<ABaseWorldObject*> RegisteredObjects,
	const FVector& ActorLocation)
{
	ABaseWorldObject* ClosestObject = nullptr;
	float ClosestDist = TNumericLimits<float>::Max();
	for (ABaseWorldObject* Object : RegisteredObjects)
	{
		if (!IsValid(Object))
		{
			continue;
		}
		
		float Distance = FVector::DistSquared(ActorLocation, Object->GetActorLocation());
		if (Distance < ClosestDist)
		{
			ClosestDist = Distance;
			ClosestObject = Object;
		}
	}
	
	return ClosestObject;
}

