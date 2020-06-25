// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PacMan.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pellet.generated.h"

UCLASS()
class ARCADEGAMES_API APellet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APellet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

protected:
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Pellet;

	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USoundBase* EatSound;

	UPROPERTY(Category = GamePlay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int Score = 100;


	APacMan* pPacMan = nullptr;
};
