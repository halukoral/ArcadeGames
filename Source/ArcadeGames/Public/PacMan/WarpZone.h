// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PacMan.h"
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "WarpZone.generated.h"

/**
 * HOW TO USE IT:
 * Just assign pDestination any actor you want.
 * It can be Warpzone too but if pDestination 
 * is nullptr then WarpZone will not work.
 */
UCLASS()
class ARCADEGAMES_API AWarpZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarpZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	
	bool bTeleportByMe = false;

	APacMan* pPacMan = nullptr;

	UPROPERTY(Category = Destination, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor*	 pDestination = nullptr;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Trigger = nullptr;

	UPROPERTY(Category = Destination, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* Arrow = nullptr;

	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USoundBase* WarpSound;
};

