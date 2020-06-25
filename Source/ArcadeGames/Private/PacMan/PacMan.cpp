// Fill out your copyright notice in the Description page of Project Settings.


#include "PacMan.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APacMan::APacMan()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PacManOPEN = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacManOPEN"));
	PacManOPEN->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = PacManOPEN;
	
	Tags.Add(FName("PacMan"));

	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &APacMan::OnOverlapBegin);
	
}

// Called when the game starts or when spawned
void APacMan::BeginPlay()
{
	Super::BeginPlay();
	bTeleported = false;
	RoundStart();
}

// Called every frame
void APacMan::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

	// Find movement direction
	const float ForwardValue = GetInputAxisValue("MoveX");
	const float RightValue = -1 * GetInputAxisValue("MoveY");

	if (ForwardValue > 0.7 || ForwardValue < -0.7)
	{
		bGoForward = true;
		bGoRight = false;
		AxisValue = ForwardValue;
	}
	else if (RightValue > 0.7 || RightValue < -0.7)
	{
		bGoRight = true;
		bGoForward = false;
		AxisValue = RightValue;
	}

	if(bGoForward)
		Move(AxisValue, DeltaTime, true);
	
	else if(bGoRight)
		Move(AxisValue, DeltaTime, false);

}

// Called to bind functionality to input
void APacMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveX");
	PlayerInputComponent->BindAxis("MoveY");
}

void APacMan::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor)
	{
		if (OtherActor->Tags.Contains("Enemy"))
		{
			UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN"));
		}
	}
}

void APacMan::RoundStart()
{
	SetActorLocation(StartLocation);
}

void APacMan::Move(float AxisValue, float DeltaTime, bool AxisForward)
{
	FVector MoveDirection;

	if (AxisForward)
	{
		// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
		MoveDirection = FVector(AxisValue, 0.f, 0.f).GetClampedToMaxSize(1.0f);
	}
	else
	{
		// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
		MoveDirection = FVector(0.f, AxisValue, 0.f).GetClampedToMaxSize(1.0f);
	}

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaTime;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
}

void APacMan::SpeedPower()
{
	MoveSpeed *= 2;

	// We can delay just like BP DELAY function
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		MoveSpeed /= 2;
	}, DelayTime, false);
}

void APacMan::Invincible()
{
	bInvincible = true;
}

