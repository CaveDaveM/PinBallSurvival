// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballCharacter.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EPinCollisionChannel.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModeClasses/PlayerHUD.h"
#include "HUD/AlwaysOnDisplay.h"

// Sets default values
APinballCharacter::APinballCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PlayerMesh"));
	RootComponent = PlayerMesh;
	PlayerMesh->SetSimulatePhysics(true);
	PlayerMesh->SetCollisionObjectType(ECC_PLAYER);
	PlayerMesh->SetCollisionResponseToAllChannels(ECR_Block);
	PlayerMesh->SetCollisionResponseToChannel(ECC_INTERACTABLE,ECR_Overlap);
	PlayerMesh->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(RootComponent);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(PlayerMesh);
	//Attach the Camera to the SpringArmComponent
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	
	PawnDetectionSphere->SetupAttachment(RootComponent);

	PlayerHUDWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerHUDWidget"));
	PlayerHUDWidget->SetupAttachment(PlayerMesh);
	PlayerHUDWidget->SetWidgetSpace(EWidgetSpace::World);
	PlayerHUDWidget->SetDrawSize(FVector2D(200.f, 80.f));
	PlayerHUDWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	PlayerHUDWidget->SetTwoSided(true);
}

// Called when the game starts or when spawned
// Called every frame
void APinballCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void APinballCharacter::BeginPlay()
{
	Super::BeginPlay();
	//the general hud.
	HUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());	
	// Get the player controller for this character
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Get the enhanced input local player subsystem and add a new input mapping context to it
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FirstPersonContext, 0);
		}
	}
	// player progression subsystem that tracks the players upgrades and world state system
	PlayerProgression = GetWorld()->GetSubsystem<UPlayerProgressionSubsystem>();
	if (PlayerProgression)
	{
		PlayerProgression->OnPlayerStats.AddDynamic(this, &APinballCharacter::UpdatePlayerStats);
		PlayerStats = PlayerProgression->GetPlayerStats();
		UE_LOG(LogLevel, Log, TEXT("Player Progression Enabled"));
	}
	else
	{
		UE_LOG(LogLevel, Warning, TEXT("Player Progression not found in StaticWindow.cpp"));
	}
	WorldState = GetWorld()->GetSubsystem<UWorldStateSubsystem>();
	if (WorldState)
	{
		WorldState->OnGameInProgress.AddUObject(this, &APinballCharacter::StartGame);
	}
	
	
	//Players Always on Display that moves around with the player
	if (AlwaysOnDisplayClass && PlayerHUDWidget)
	{
		PlayerHUDWidget->SetWidgetClass(AlwaysOnDisplayClass);
		PlayerHUDWidget->InitWidget();
		AlwaysOnDisplayHud = Cast<UAlwaysOnDisplay>(PlayerHUDWidget->GetUserWidgetObject());
	}
	UpdateHudStats();
	
	//Load Visual Effects
	if (SpeedEffectClass)
	{
		SpeedEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
			SpeedEffectClass,
			PlayerMesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			false,
			false);
	}

	// Display a debug message for five seconds. 
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, 
		TEXT("We are using AdventureCharacter."));
	
	GetWorld()->GetTimerManager().SetTimer(
		UpdateCurrentSpeed_TimeHandle,
		this,
		&APinballCharacter::UpdateCurrentSpeed,
		.2,
		true);
	
}

void APinballCharacter::StartGame(EGamePhase GameState)
{
	if (GameState == EGamePhase::Playing)
	{
		FVector EndTutorialLocation = FVector(2568.0, -16135.0, 20.0 );
		FRotator EndTutorialRotation = FRotator(0.0f, 0.0f, -180.0f);
		SetActorLocationAndRotation(
			EndTutorialLocation,
			EndTutorialRotation,
			false,
			nullptr,
			ETeleportType::ResetPhysics);
		
		CurrentAmmo = 40;
		Health = PlayerStats.MaxHealth;
		UpdateHudStats();
	}
}



void APinballCharacter::ShotFired()
{
	Super::ShotFired();
	UpdateHudStats();
}
void APinballCharacter::UpdatePlayerStats(const FPlayerStats NewPlayerStats)
{
	PlayerStats = NewPlayerStats;
}

void APinballCharacter::UpdateHudStats()
{
	Super::UpdateHudStats();
	if (AlwaysOnDisplayHud)
	{
		AlwaysOnDisplayHud->UpdateHealthBar(Health, PlayerStats.MaxHealth);
		AlwaysOnDisplayHud->UpdateAmmoText(CurrentAmmo,MaxAmmo);	
	}
}

//MOVEMENT LOGIC

void APinballCharacter::UpdateCurrentSpeed()
{
	FVector MovementVectors = GetVelocity();
	CurrentSpeed = MovementVectors.Size();
	CalculateDamage();
	if (SpeedEffect && CurrentSpeed > 1000.0f && CurrentSpeed < 1999.0f)
	{
		SpeedEffect->Activate();
		SpeedEffect->SetVariableLinearColor(
		TEXT("User.Color"),
		FLinearColor(1.0f,0.0f,0.0f,1.0f));
	}
	else if (SpeedEffect && CurrentSpeed > 2000.0f )
	{
		SpeedEffect->Activate();
		SpeedEffect->SetVariableLinearColor(
			TEXT("User.Color"),
			FLinearColor(0.0f,0.0f,1.0f,1.0f));
	}
	else if (SpeedEffect && CurrentSpeed < 1000.0f && SpeedEffect->IsActive())
	{
		SpeedEffect->Deactivate();
	}
}

void APinballCharacter::CalculateDamage()
{
	TotalDamage = PlayerStats.ProjectileDamage + (CurrentSpeed * DamageScaling);
}



void APinballCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector);
}


void APinballCharacter::DoMove(FVector2D MoveVector)
{
	FVector Vectors;
	Vectors.X = (MoveVector.X * 50) * PlayerStats.MoveSpeedScalar;
	Vectors.Y = (MoveVector.Y * 50) * PlayerStats.MoveSpeedScalar;
	Vectors.Z = 0.0f;
	PlayerMesh->AddImpulse(Vectors);
}


void APinballCharacter::ApplyForceToPlayer(FVector ForceToApply)
{
	PlayerMesh->SetPhysicsLinearVelocity(ForceToApply);
}
//HUD
void APinballCharacter::OpenGameMenu()
{
	HUD->ToggleInGameMenu();
}
//PlayerHealing
void APinballCharacter::ApplyHealing(float HealAmount)
{
	Super::ApplyHealing(HealAmount);
	if (Health + HealAmount > PlayerStats.MaxHealth)
	{
		Health = PlayerStats.MaxHealth;
	}
	else
	{
		Health += HealAmount;
	}
	UpdateHudStats();
}
// Called to bind functionality to input
void APinballCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APinballCharacter::MoveInput);
		EnhancedInputComponent->BindAction(GameMenuAction, ETriggerEvent::Started, this , &APinballCharacter::OpenGameMenu);

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Red,TEXT("Move Input Component invalid"));
	}
}


