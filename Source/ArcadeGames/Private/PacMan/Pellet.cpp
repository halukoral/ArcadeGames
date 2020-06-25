// Fill out your copyright notice in the Description page of Project Settings.


#include "Pellet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APellet::APellet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Pellet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pellet"));
	RootComponent = Pellet;
	OnActorBeginOverlap.AddDynamic(this, &APellet::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APellet::BeginPlay()
{
	pPacMan = Cast<APacMan>(UGameplayStatics::GetActorOfClass(this, APacMan::StaticClass()));
	Super::BeginPlay();
}

// Called every frame
void APellet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APellet::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->Tags.Contains("PacMan"))
		{
			if (pPacMan)
				pPacMan->AddScore(Score);

			if (EatSound)
				UGameplayStatics::PlaySoundAtLocation(this, EatSound, GetActorLocation());

			Destroy();
		}
	}
}

