// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManEnemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APacManEnemy::APacManEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = Enemy;
	Enemy = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enemy"));
	EnemyVulnarable = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyVulnarable"));
	Tags.Add(FName("Enemy"));

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	//Register Events
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &APacManEnemy::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &APacManEnemy::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APacManEnemy::BeginPlay()
{
	pPacMan = Cast<APacMan>(UGameplayStatics::GetActorOfClass(this, APacMan::StaticClass()));
	Super::BeginPlay();
}

// Called every frame
void APacManEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APacManEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APacManEnemy::Vulnarable(bool b)
{
	Enemy->ToggleVisibility(!b);
	EnemyVulnarable->ToggleVisibility(b);
	bVulnarable = b;
	
	// We can delay just like BP DELAY function
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		Enemy->ToggleVisibility(b);
		EnemyVulnarable->ToggleVisibility(!b);
		bVulnarable = !b;
		}, DelayTime, false);
}

void APacManEnemy::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->Tags.Contains("PacMan"))
		{
			UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN"));
		}
	}
}

void APacManEnemy::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

