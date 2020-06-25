// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PacMan.generated.h"

UCLASS(Blueprintable)
class ARCADEGAMES_API APacMan : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APacMan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*void MoveUp(float AxisValue);
	void MoveRight(float AxisValue);*/
	
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void RoundStart();

	void Move(float AxisValue, float DeltaTime, bool AxisForward);

	UFUNCTION(BlueprintCallable)
	void AddScore(int i) { iScore += i; }

	void SpeedPower();

	void Invincible();

	void SetTeleported(bool b) { bTeleported = b; }
	bool IsTeleported() { return bTeleported; }

private:

	// Input variables
	FVector				MovementInput;
	FTimerHandle		TimerHandle;
	APlayerController*	PlayerController;

	float AxisValue;

	bool  bGoRight;
	bool  bGoForward;
	bool  bInvincible;
	bool  bTeleported;

	int   iLive = 3;
	int   iScore = 0;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PacManOPEN;

	UPROPERTY(Category = GamePlay, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DelayTime = 5.f;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	FVector StartLocation;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 2000.0f;

};
