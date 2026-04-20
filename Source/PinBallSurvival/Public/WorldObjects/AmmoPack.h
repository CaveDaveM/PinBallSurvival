// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/WeaponInterface.h"
#include "WorldObjects/BaseWorldObject.h"
#include "AmmoPack.generated.h"

/**
 * 
 */
UCLASS()
class PINBALLSURVIVAL_API AAmmoPack : public ABaseWorldObject, public IWeaponInterface
{
	GENERATED_BODY()
	
};
