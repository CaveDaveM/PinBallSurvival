// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballComponents/Bumper.h"

#include "PinballCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ABumper::ABumper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BumperMesh = CreateDefaultSubobject<UStaticMeshComponent>("BumperMesh");
	SetRootComponent(BumperMesh);
	
	BumperSphere = CreateDefaultSubobject<USphereComponent>("BumperSphere");
	BumperSphere->SetupAttachment(BumperMesh);
	BumperSphere->SetGenerateOverlapEvents(true);
	BumperSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BumperSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	

}

// Called when the game starts or when spawned
void ABumper::BeginPlay()
{
	Super::BeginPlay();
	BumperSphere->OnComponentBeginOverlap.AddDynamic(this,&ABumper::OnOverlapBegin);
}

void ABumper::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APinballCharacter* PlayerCharacter = Cast <APinballCharacter>(OtherActor))
	{
		FVector HitNormal = SweepResult.ImpactNormal;
		PushAwayActor(PlayerCharacter, HitNormal);
	}
}

void ABumper::PushAwayActor(APinballCharacter* OverlappedActor, FVector HitNormal)
{
#if UE_BUILD_DEVELOPMENT
	GEngine->AddOnScreenDebugMessage(-1,3.0f, FColor::Green,TEXT("Bumper Is Overlapping"));
#endif
	
	/*
	FVector OverlappedActorsVector = OverlappedActor->GetVelocity();
	FVector ReflectionDirection = ReflectVector(OverlappedActorsVector,HitNormal,1.0f);
	FVector ReflectionForce = ReflectionDirection * OverlappedActor->GetActorForwardVector() * 50.0f;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
	FString::Printf(TEXT("Vector: %s"), *ReflectionForce.ToString()));
	
	OverlappedActor->ApplyForceToPlayer(ReflectionForce);
	*/
	
	FVector BumperCentre = GetActorLocation();

	FVector Normal = (HitNormal - BumperCentre).GetSafeNormal();
	
	FVector IncomingVelocity = OverlappedActor->GetVelocity();
	float Dot = FVector::DotProduct(IncomingVelocity, Normal);
	FVector ReflectedVelocity = IncomingVelocity - 2.f * Dot * Normal;
	
	if (ReflectedVelocity.Size() < MinBumpSpeed)
	{
		ReflectedVelocity = Normal * MinBumpSpeed;
	}
		
	//OverlappedActor->ApplyForceToPlayer(ReflectedVelocity);
	OverlappedActor->PlayerMesh->SetPhysicsLinearVelocity(ReflectedVelocity);
	
}

/*FVector ABumper::ReflectVector(FVector Direction, FVector WallNormal, float Restitution)
{
	// WallNormal must be normalised
	//code from the unreal implementation, slightly adjusted
	FVector SafeNormal(WallNormal.GetSafeNormal());

	return Direction - (1.0f + Restitution) * (Direction | SafeNormal) * SafeNormal;
}*/

// Called every frame
void ABumper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

