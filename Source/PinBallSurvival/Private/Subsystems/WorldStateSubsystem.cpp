// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/WorldStateSubsystem.h"

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
	RegisteredWorldObjects.Add(SpawnedObject);
}

void UWorldStateSubsystem::UnregisterWorldObject(ABaseWorldObject* SpawnedObject)
{
	RegisteredWorldObjects.Remove(SpawnedObject);
}

