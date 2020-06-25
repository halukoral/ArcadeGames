// Fill out your copyright notice in the Description page of Project Settings.

#include "PONG_Ball.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


// Sets default values
APONG_Ball::APONG_Ball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &APONG_Ball::OnOverlapBegin);
	//OnActorEndOverlap.AddDynamic(this, &APONG_Ball::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APONG_Ball::BeginPlay()
{
	Super::BeginPlay();
	RoundStart();
}

void APONG_Ball::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Ensure the UnusedHandle timer is cleared by using the timer handle
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	// Alternatively you can clear ALL timers that belong to this (Actor) instance.
	// GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void APONG_Ball::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	//we may have many balls so that check other is not self
	if (OtherActor && (OtherActor != this))
	{
		FVector vel = MeshComponent->GetPhysicsLinearVelocity();
		
		if (OtherActor->Tags.Contains("Paddle"))
		{
			FVector otherVel = OtherActor->FindComponentByClass<UStaticMeshComponent>()
										 ->GetPhysicsLinearVelocity();
			vel.Y *= -1.0f;
			vel.X = otherVel.X * FMath::RandRange(0.75f, 1.f);
			if (fabs(otherVel.X) <= 0.1f)
				vel.X = 1000 * FMath::RandRange(-0.75f, 1.0f);
			
			if (HitSound)
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
		else if (OtherActor->Tags.Contains("Bound"))
		{
			vel.X *= -1.0f * FMath::RandRange(0.75f, 1.f);

			if (HitSound)
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
		else if (OtherActor->Tags.Contains("GOAL1P"))
		{
			if (GoalSound)
				UGameplayStatics::PlaySoundAtLocation(this, GoalSound, GetActorLocation());

			++Player1Score;
			MeshComponent->SetSimulatePhysics(false);
			Speed *= -1;
			Delay(FName("RoundStart"));
		}
		else if (OtherActor->Tags.Contains("GOAL2P"))
		{
			if (GoalSound)
				UGameplayStatics::PlaySoundAtLocation(this, GoalSound, GetActorLocation());

			++Player2Score;
			MeshComponent->SetSimulatePhysics(false);
			Speed *= -1;
			Delay(FName("RoundStart"));
		}

		MeshComponent->SetPhysicsLinearVelocity(vel);
	}
}

void APONG_Ball::Delay(const FName& FuncName)
{
	// Binding the function with specific variables
	TimerDelegate.BindUFunction(this, FuncName);
	// Calling MyUsefulFunction after TimerDelay seconds without looping
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, TimerDelay, false);
}

void APONG_Ball::RoundStart()
{
	MeshComponent->SetSimulatePhysics(true);
	SetActorLocation(FVector::ZeroVector);
	FVector NewVel = FVector(0.0f, Speed, 0.0f);
	MeshComponent->SetPhysicsLinearVelocity(NewVel);
}

// Called every frame
void APONG_Ball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Player1Score >= MaxScore || Player2Score >= MaxScore)
		UKismetSystemLibrary::QuitGame(GetWorld(),0, EQuitPreference::Quit, true);
}

