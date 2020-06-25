// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerfulPellet.h"
#include "PacManEnemy.h"
#include "Kismet/GameplayStatics.h"

APowerfulPellet::APowerfulPellet()
{
	OnActorBeginOverlap.AddDynamic(this, &APowerfulPellet::OnOverlapBegin);
}

void APowerfulPellet::BeginPlay()
{
	pPacMan = Cast<APacMan>(UGameplayStatics::GetActorOfClass(this, APacMan::StaticClass()));
	Super::BeginPlay();
}

void APowerfulPellet::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->Tags.Contains("PacMan"))
		{
			if (pPacMan)
				pPacMan->AddScore(Score);

			if (EatSound)
				UGameplayStatics::PlaySoundAtLocation(this, EatSound, GetActorLocation());

			UGameplayStatics::GetAllActorsOfClass(this, APacManEnemy::StaticClass(),Enemies);

			for (auto e : Enemies)
			{
				APacManEnemy* enemy = Cast<APacManEnemy>(e);
				if (enemy)
					enemy->Vulnarable(true);
			}

			Destroy();
		}
	}
}
