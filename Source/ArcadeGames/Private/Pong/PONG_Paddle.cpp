 // Fill out your copyright notice in the Description page of Project Settings.

#include "PONG_Paddle.h"
#include "EngineUtils.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
APONG_Paddle::APONG_Paddle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MainCamera = CreateDefaultSubobject<AActor>(TEXT("Camera"));
	Tags.Add(FName("Paddle"));

}

// Called when the game starts or when spawned
void APONG_Paddle::BeginPlay()
{
	Super::BeginPlay();	
	//Take control of the default Player
	if (PlayerEnum == EPlayer::P_P1) 
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		APawn* Pawn = PlayerController->GetPawn();
		if(Pawn != nullptr)
			Pawn->Destroy();
		PlayerController->Possess(this);
		//AutoPossessPlayer = EAutoReceiveInput::Player0;
		if (MainCamera != nullptr)
			PlayerController->SetViewTargetWithBlend(MainCamera);
	}
	else if (PlayerEnum == EPlayer::P_P2)
	{
		APlayerController* PlayerController = UGameplayStatics::CreatePlayer(GetWorld());
		APawn* Pawn = PlayerController->GetPawn();
		if (Pawn != nullptr)
			Pawn->Destroy();
		PlayerController->Possess(this);
		if (PlayerController == nullptr)
			UE_LOG(LogTemp, Warning, TEXT("Your message"));
		if (MainCamera != nullptr)
			PlayerController->SetViewTargetWithBlend(MainCamera);
	}
	else if (PlayerEnum == EPlayer::P_AI)
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APONG_Ball::StaticClass(), Actors);
		Ball = Cast<APONG_Ball>(Actors[0]);
	}
}

void APONG_Paddle::Move(float AxisValue)
{
	// Move at 100 units per second forward or backward
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f) * MovementSpeed;
}

void APONG_Paddle::MoveWithSetActorLocation(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
	//NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
	if (NewLocation.X > -1320.f && NewLocation.X < 1320.f)
		SetActorLocation(NewLocation);
}

void APONG_Paddle::MoveWithAddForce(float DeltaTime)
{
	if (GetActorLocation().X > -1300.f && GetActorLocation().X < 1300.f)
	{
		FVector Force = FVector(MovementInput.X * DeltaTime * ForceMultiplier, 0.0f, 0.0f);
		MeshComponent->AddForce(Force);
	}
	else
	{
		MoveWithSetActorLocation(DeltaTime);
	}
}

void APONG_Paddle::AIMovement()
{
	float XDifference = Ball->GetActorLocation().X - GetActorLocation().X;
	float XDifferenceWithError = FMath::Clamp(XDifference, -1.0f * AIPaddleMovementError, AIPaddleMovementError);
	if (XDifference < -22 || XDifference > 22)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), XDifference);
		MeshComponent->AddImpulse( FVector(XDifferenceWithError * AIPaddleSpeed,0.0f,0.0f) );
	}

}

void APONG_Paddle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PlayerEnum == EPlayer::P_AI) 
	{
		bAllowEditPlayer = false;
		bAllowEditAI = true;
	}
	else
	{
		bAllowEditPlayer = true;
		bAllowEditAI = false;
	}
}

// Called every frame
void APONG_Paddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerEnum == EPlayer::P_AI)
	{
		AIMovement();
	}

    // Handle movement based on our "Move" axes
    {
        if (!MovementInput.IsZero())
        {
			if (MovementEnum == EMovementMethod::MV_Location)
				MoveWithSetActorLocation(DeltaTime);
			else
				MoveWithAddForce(DeltaTime);
        }
    }
}

// Called to bind functionality to input
void APONG_Paddle::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	if (PlayerEnum == EPlayer::P_P1)
	{
		InputComponent->BindAxis("Pong_P1_Move", this, &APONG_Paddle::Move);
		UE_LOG(LogTemp, Warning, TEXT("P1"));
	}
	else if (PlayerEnum == EPlayer::P_P2)
	{
		InputComponent->BindAxis("Pong_P2_Move", this, &APONG_Paddle::Move);
		UE_LOG(LogTemp, Warning, TEXT("P2"));
	}
}

