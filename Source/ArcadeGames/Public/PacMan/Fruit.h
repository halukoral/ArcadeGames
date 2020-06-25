// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacMan/Pellet.h"
#include "Fruit.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EPowerType : uint8
{
	P_Speed		 	UMETA(DisplayName = "Speed Power"),
	P_Invincible	UMETA(DisplayName = "Invincible")
};

/**
 * 
 */
UCLASS()
class ARCADEGAMES_API AFruit : public APellet
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AFruit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) override;

private:
	UPROPERTY(Category = Power, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPowerType ePower;
};
