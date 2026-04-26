// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/AmmoPack.h"

#include "PinballCharacter.h"
#include "Components/SphereComponent.h"

class APinballCharacter;

AAmmoPack::AAmmoPack()
{
}

void AAmmoPack::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPack::OnOverlapBegin);
}

void AAmmoPack::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		if (IWeaponInterface* Weapon = Cast<IWeaponInterface>(OtherActor))
		{
			Weapon->AddAmmo(AmmoPackAmount);
			Destroy();
		}
	}
}

void AAmmoPack::SetObjectRarity(EObjectRarity Rarity)
{
	Super::SetObjectRarity(Rarity);
	switch (Rarity) {
	case EObjectRarity::Common:
		{
			AmmoPackAmount = 40;
			break;
		}
	case EObjectRarity::Rare:
		{
			AmmoPackAmount = 80;
			break;
		}
	case EObjectRarity::Unique:
		{
			AmmoPackAmount = 120;
			break;
		}
	}
}
