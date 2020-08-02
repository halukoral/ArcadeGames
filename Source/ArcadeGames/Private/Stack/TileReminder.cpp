// Fill out your copyright notice in the Description page of Project Settings.


#include "TileReminder.h"

// Sets default values
ATileReminder::ATileReminder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATileReminder::BeginPlay()
{
	Super::BeginPlay();
	
}


