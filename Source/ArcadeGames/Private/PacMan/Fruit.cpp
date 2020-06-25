// Fill out your copyright notice in the Description page of Project Settings.


#include "Fruit.h"
#include "Kismet/GameplayStatics.h"

AFruit::AFruit()
{
	OnActorBeginOverlap.AddDynamic(this, &AFruit::OnOverlapBegin);
}

void AFruit::BeginPlay()
{
	pPacMan = Cast<APacMan>(UGameplayStatics::GetActorOfClass(this, APacMan::StaticClass()));
	Super::BeginPlay();
}

void AFruit::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->Tags.Contains("PacMan"))
		{
			UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN"));

			if (pPacMan)
				pPacMan->AddScore(Score);

			if (EatSound)
				UGameplayStatics::PlaySoundAtLocation(this, EatSound, GetActorLocation());

			if (ePower == EPowerType::P_Speed)
				pPacMan->SpeedPower();

			else if (ePower == EPowerType::P_Invincible)
				pPacMan->Invincible();

			Destroy();
		}
	}
}
