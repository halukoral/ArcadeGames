// Fill out your copyright notice in the Description page of Project Settings.


#include "StackSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AStackSpawner::AStackSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrow"));
	RightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrow"));
	SceneComponent->SetupAttachment(RootComponent);
	LeftArrow->SetupAttachment(SceneComponent);
	RightArrow->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AStackSpawner::BeginPlay()
{
	Super::BeginPlay();

	auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PC->SetViewTargetWithBlend(this);

	ATile* tile = nullptr;
	for (int i = 0; i < 15; ++i) {
		const FVector Loc = SceneComponent->GetComponentLocation();
		const FRotator Rot = SceneComponent->GetComponentRotation();
		if(TileClass)
			tile = GetWorld()->SpawnActor<ATile>(TileClass, Loc, Rot);

		if(tile)
			tile->StaticMesh->SetVectorParameterValueOnMaterials(TEXT("BASE"), SetColor());
		

		SceneComponent->AddWorldOffset(FVector(0, 0, 10));
	}
	PrevTile = tile;
	SpawnTile();
}

// Called every frame
void AStackSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(CurrentTile && bCanMove)
		MoveTile(DeltaTime, speed);

}

FVector AStackSpawner::SetColor() {
	H += 3.f;
	if (V > 0.8f) {
		S -= 0.02;
		if (S < 0.7) {
			S = 0.8;
			V = 0.2;
			H = UKismetMathLibrary::RandomFloatInRange(0.0f, 389.0f);
		}
	}
	else 
		V += 0.05f;

	return GetColor();
	
}

FVector AStackSpawner::GetColor() {
	return UKismetMathLibrary::HSVToRGB(H, S, V);
}

void AStackSpawner::SpawnTile() {
	if (bIsOnLeft) {
		FVector Loc = LeftArrow->GetComponentLocation();
		if (TileClass) {
			Loc.Y = PrevTile->GetActorLocation().Y;
			CurrentTile = GetWorld()->SpawnActor<ATile>(TileClass, Loc, FRotator(0, 0, 0));
		}
	}
	else {
		FVector Loc = RightArrow->GetComponentLocation();
		if (TileClass) {
			Loc.X = PrevTile->GetActorLocation().X;
			CurrentTile = GetWorld()->SpawnActor<ATile>(TileClass, Loc, FRotator(0, 0, 0));
		}
	}

	CurrentTile->SetActorScale3D(PrevTile->GetActorScale3D());
	bCanMove = true;
	CurrentColor = SetColor();
	CurrentTile->StaticMesh->SetVectorParameterValueOnMaterials(TEXT("Base"), CurrentColor);

}

void AStackSpawner::MoveTile(float deltaTime, float speed) {
	if (bIsOnLeft)
		CurrentTile->AddActorWorldOffset(FVector(deltaTime * speed * -1, 0, 0));
	else
		CurrentTile->AddActorWorldOffset(FVector(0, deltaTime * speed * -1, 0));
}

void AStackSpawner::StackFire() {
	bCanMove = false;
	if (CurrentTile && PrevTile && CurrentTile->IsOverlappingActor(PrevTile)) {
		SceneComponent->AddWorldOffset(FVector(0.f, 0.f, 10.f));
		if (bIsOnLeft) {
			float remainderScaleX;
			bool bCanSpawn = IntersectionLeft(remainderScaleX);
			RemainderLeft(bCanSpawn, remainderScaleX);
			bIsOnLeft = false;
		}
		else {
			float remainderScaleY;
			bool bCanSpawn = IntersectionRight(remainderScaleY);
			RemainderRight(bCanSpawn, remainderScaleY);
			bIsOnLeft = true;
		}
		PrevTile = CurrentTile;
		SpawnTile();
	}
	else {
		EndGame();
	}
}

void AStackSpawner::EndGame() {
	CurrentTile->StaticMesh->SetSimulatePhysics(true);
}

bool AStackSpawner::IntersectionLeft(float& remainderScaleX) {
	float tolerance = 4.0;
	float currLocX = CurrentTile->GetActorLocation().X;
	float prevLocX = PrevTile->GetActorLocation().X;
	float PrevTileScaleX = PrevTile->GetActorScale3D().X;

	if (UKismetMathLibrary::Abs(currLocX - prevLocX) <= tolerance) {
		FVector Loc = CurrentTile->GetActorLocation();
		Loc.X = prevLocX;
		CurrentTile->SetActorLocation(Loc);
		remainderScaleX = 0.f;
		return false;
	}
	else {
		FVector scale = CurrentTile->GetActorScale3D();
		scale.X = PrevTileScaleX - UKismetMathLibrary::Abs((currLocX - prevLocX) / 100.f);
		CurrentTile->SetActorScale3D(scale);
		
		FVector Loc = CurrentTile->GetActorLocation();
		Loc.X = (currLocX + prevLocX) / 2.f;
		CurrentTile->SetActorLocation(Loc);
		remainderScaleX = ((currLocX - prevLocX) / 100.f);
		return true;

	}
}

bool AStackSpawner::IntersectionRight(float& remainderScaleY) {
	float tolerance = 4.0;
	float currLocY = CurrentTile->GetActorLocation().Y;
	float prevLocY = PrevTile->GetActorLocation().Y;
	float PrevTileScaleY = PrevTile->GetActorScale3D().Y;

	if (UKismetMathLibrary::Abs(currLocY - prevLocY) <= tolerance) {
		FVector Loc = CurrentTile->GetActorLocation();
		Loc.Y = prevLocY;
		CurrentTile->SetActorLocation(Loc);
		remainderScaleY = 0.f;
		return false;
	}
	else {
		FVector scale = CurrentTile->GetActorScale3D();
		scale.Y = PrevTileScaleY - UKismetMathLibrary::Abs((currLocY - prevLocY) / 100.f);
		CurrentTile->SetActorScale3D(scale);

		FVector Loc = CurrentTile->GetActorLocation();
		Loc.Y = (currLocY + prevLocY) / 2.f;
		CurrentTile->SetActorLocation(Loc);
		remainderScaleY = ((currLocY - prevLocY) / 100.f);
		return true;

	}
}

void AStackSpawner::RemainderRight(bool bCanSpawn, float remainderScaleY) {
	if (bCanSpawn) {
		FVector Loc = CurrentTile->GetActorLocation();
		FVector Scale = CurrentTile->GetActorScale3D();
		FVector origin;
		FVector BoxExtent;
		PrevTile->GetActorBounds(false, origin, BoxExtent);
		Loc.Y += BoxExtent.Y * UKismetMathLibrary::SignOfFloat(remainderScaleY);
		Scale.Y = UKismetMathLibrary::Abs(remainderScaleY);
		ATileReminder* tempTile = nullptr;
		if (RemainderClass)
			tempTile = GetWorld()->SpawnActor<ATileReminder>(RemainderClass, Loc, FRotator(0, 0, 0));
		if (tempTile) {
			tempTile->SetActorScale3D(Scale);
			tempTile->StaticMesh->SetVectorParameterValueOnMaterials(TEXT("BASE"), CurrentColor);
		}
	}
}

void AStackSpawner::RemainderLeft(bool bCanSpawn, float remainderScaleX) {
	if (bCanSpawn) {
		FVector Loc = CurrentTile->GetActorLocation();
		FVector Scale = CurrentTile->GetActorScale3D();
		FVector origin;
		FVector BoxExtent;
		PrevTile->GetActorBounds(false, origin, BoxExtent);
		Loc.X += BoxExtent.X * UKismetMathLibrary::SignOfFloat(remainderScaleX);
		Scale.X = UKismetMathLibrary::Abs(remainderScaleX);
		ATileReminder* tempTile = nullptr;
		if (RemainderClass) {
			tempTile = GetWorld()->SpawnActor<ATileReminder>(RemainderClass, Loc, FRotator(0, 0, 0));
		}
		
		if (tempTile) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("ALO"), false);
			tempTile->SetActorScale3D(Scale);
			tempTile->StaticMesh->SetVectorParameterValueOnMaterials(TEXT("BASE"), CurrentColor);
		}
	}
}