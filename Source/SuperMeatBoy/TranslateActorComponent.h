// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TranslateActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUPERMEATBOY_API UTranslateActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTranslateActorComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	AActor* Actor;
	UPROPERTY(EditAnywhere)
		float Speed = 150.0f;
	UPROPERTY(EditAnywhere)
		float DirectThreshold = 10.0f;
	TArray<AActor*> ActorArray;
	TArray<FVector> PointArray;

	virtual void SetNextDirection();
	virtual void SetAnimation(float DeltaTime);

	float LeftTime;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int CurPoint, NextPoint;
	FVector CurDirection;
	
};
