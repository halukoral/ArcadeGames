// Fill out your copyright notice in the Description page of Project Settings.


#include "WarpZone.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWarpZone::AWarpZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	RootComponent = Arrow;
	//Register Events
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AWarpZone::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AWarpZone::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AWarpZone::BeginPlay()
{
	pPacMan = Cast<APacMan>(UGameplayStatics::GetActorOfClass(this, APacMan::StaticClass()));
	Super::BeginPlay();
}

void AWarpZone::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->Tags.Contains("PacMan") && pDestination && !pPacMan->IsTeleported())
		{
			if (WarpSound)
				UGameplayStatics::PlaySoundAtLocation(this, WarpSound, GetActorLocation());

			if (Cast<AWarpZone>(pDestination))
			{
				bTeleportByMe = true;
				pPacMan->SetTeleported(true);
			}
			OtherActor->TeleportTo(pDestination->GetActorLocation(), pDestination->GetActorRotation());
		}
	}
}

void AWarpZone::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->Tags.Contains("PacMan") && !bTeleportByMe && pPacMan->IsTeleported())
		{
			pPacMan->SetTeleported(false);
			if (Cast<AWarpZone>(pDestination))
			{
				Cast<AWarpZone>(pDestination)->bTeleportByMe = false;
			}
		}
	}
}

