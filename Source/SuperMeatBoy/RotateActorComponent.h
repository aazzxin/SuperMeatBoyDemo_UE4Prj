// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "RotateActorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUPERMEATBOY_API URotateActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotateActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AActor* Actor;

	UPROPERTY(EditAnywhere)
	AActor* TargetActor;

	UPROPERTY(EditAnywhere)
		float Speed = 30.0f;

	virtual void SetAnimation(float DeltaTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FRotator Rotator;
	
};
