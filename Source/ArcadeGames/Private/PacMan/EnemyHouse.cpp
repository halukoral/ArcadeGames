// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHouse.h"

// Sets default values
AEnemyHouse::AEnemyHouse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Area = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Area"));
	Entrance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Entrance"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	//Register Events
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemyHouse::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AEnemyHouse::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AEnemyHouse::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyHouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyHouse::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN"));

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN INNER"));

		if (OtherActor->Tags.Contains("PacMan"))
		{
			Entrance->SetCollisionResponseToAllChannels(ECR_Block);
			UE_LOG(LogTemp, Warning, TEXT("PacMan"));

		}
		else if (OtherActor->Tags.Contains("Enemy"))
		{
			Entrance->SetCollisionResponseToAllChannels(ECR_Ignore);
			UE_LOG(LogTemp, Warning, TEXT("Enemy"));

		}
	}
}

void AEnemyHouse::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

