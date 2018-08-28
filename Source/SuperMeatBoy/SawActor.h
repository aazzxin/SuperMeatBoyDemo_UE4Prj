// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelActor.h"
#include "SawActor.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API ASawActor : public ALevelActor
{
	GENERATED_BODY()
public:
	ASawActor();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OverlapAction(ASuperMeatBoyCharacter* Character) override;
	void UpdateAnimation();
	
private:
	FRotator CurrentRotator;

	const float RotateSpeed = 60.0f;
	const float ImpactSpeed = 100.0f;
};
