// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/WorldStateSubsystem.h"

#include "Interfaces/HealthInterface.h"
#include "Interfaces/WeaponInterface.h"

void UWorldStateSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UWorldStateSubsystem::Deinitialize()
{
	Super::Deinitialize();
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
	RegisteredHealingObjects.Remove(SpawnedObject);
}

