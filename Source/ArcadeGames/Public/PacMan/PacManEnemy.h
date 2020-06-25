// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PacMan.h"
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "PacManEnemy.generated.h"

UCLASS()
class ARCADEGAMES_API APacManEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APacManEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Vulnarable(bool b);

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Enemy;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* EnemyVulnarable;

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Trigger;

	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USoundBase* EatenSound;

	UPROPERTY(Category = GamePlay, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int Score = 400;

	UPROPERTY(Category = GamePlay, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DelayTime = 5.f;

	bool			bVulnarable		= false;
	APacMan*		pPacMan			= nullptr;
	FTimerHandle    TimerHandle;

};
