// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "PONG_Ball.generated.h"

UCLASS()
class ARCADEGAMES_API APONG_Ball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APONG_Ball();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	// overlap end function

	void Delay(const FName& FuncName);

	// It’s important to make your function with UFUNCTION() 
	// macro in order to have it bind correctly with the timer. 
	// Otherwise an error will be thrown and can crash the engine.
	UFUNCTION()
	void RoundStart();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* MeshComponent;
	
	UFUNCTION(BlueprintCallable)
	int Score1P() { return Player1Score; }
	
	UFUNCTION(BlueprintCallable)
	int Score2P() { return Player2Score; }

private:
	UPROPERTY(EditAnywhere)
	int MaxScore = 10;
	int Player1Score = 0;
	int Player2Score = 0;
	
	UPROPERTY(EditAnywhere)
	float Speed = -2000.0f;

	float TimerDelay = 0.50f;

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* GoalSound;
};
