// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballCharacter.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APinballCharacter::APinballCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PlayerMesh"));
	RootComponent = PlayerMesh;
	PlayerMesh->SetSimulatePhysics(true);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(RootComponent);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(PlayerMesh);
		
	//Attach the Camera to the SpringArmComponent
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APinballCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Called when the game starts or when spawned

	Super::BeginPlay();

	check(GEngine != nullptr);

	// Get the player controller for this character
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Get the enhanced input local player subsystem and add a new input mapping context to it
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FirstPersonContext, 0);
		}
	}

	// Display a debug message for five seconds. 
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using AdventureCharacter."));
	
	GetWorld()->GetTimerManager().SetTimer(
		UpdateCurrentSpeed_TimeHandle,
		this,
		&APinballCharacter::UpdateCurrentSpeed,
		0.2,
		true);
	
}

void APinballCharacter::UpdateCurrentSpeed()
{
	FVector MovementVectors = GetVelocity();
	float MovementSpeed = MovementVectors.Size();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,  
		FString::Printf(TEXT("MyValue: %f"), MovementSpeed));
}


void APinballCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector);
}

// Called every frame
void APinballCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void APinballCharacter::DoMove(FVector2D MoveVector)
{
	FVector Vectors;
	Vectors.X = MoveVector.X * 100;
	Vectors.Y = MoveVector.Y * 100;
	Vectors.Z = 0.0f;
	PlayerMesh->AddImpulse(Vectors);
}

void APinballCharacter::ApplyForceToPlayer(FVector ForceToApply)
{
	PlayerMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	PlayerMesh->AddImpulse(ForceToApply);
}


// Called to bind functionality to input
void APinballCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APinballCharacter::MoveInput);
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Red,TEXT("valid"));

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Red,TEXT("Move Input Component invalid"));
	}
}

