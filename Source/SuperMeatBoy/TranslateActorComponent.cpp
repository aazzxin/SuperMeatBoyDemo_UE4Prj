// Fill out your copyright notice in the Description page of Project Settings.

#include "TranslateActorComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UTranslateActorComponent::UTranslateActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTranslateActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Actor = GetOwner();
    //GetOwner()->GetAllChildActors(ActorArray);
	Actor->GetAttachedActors(ActorArray);
	for (auto Actor : ActorArray) {
		PointArray.Add(Actor->GetActorLocation());
	}
	if (PointArray.Num() > 1) {
		CurDirection = (PointArray[1] - PointArray[0]);
		LeftTime = FVector::Distance(PointArray[1] , PointArray[0]) / Speed;
		CurDirection.Normalize();
		NextPoint = 1;
	}
	if (PointArray.Num() > 0)
		Actor->SetActorLocation(PointArray[0]);
}


void UTranslateActorComponent::SetNextDirection()
{
	CurPoint++;
	CurPoint %= PointArray.Num();
	NextPoint++;
	NextPoint %= PointArray.Num();
	CurDirection = (PointArray[NextPoint] - PointArray[CurPoint]);
	LeftTime = FVector::Distance(PointArray[NextPoint], PointArray[CurPoint]) / Speed;
	CurDirection.Normalize();
	Actor->SetActorLocation(PointArray[CurPoint]);
}

void UTranslateActorComponent::SetAnimation(float DeltaTime)
{
	float Dist = FVector::DistSquared(PointArray[NextPoint], Actor->GetActorLocation());
	if (LeftTime <= 0) {
		SetNextDirection();
		return;
	}
	LeftTime -= DeltaTime;
	FVector Target = Actor->GetActorLocation() + Speed * DeltaTime* CurDirection;
	Actor->SetActorLocation(Target);
}

// Called every frame
void UTranslateActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetAnimation(DeltaTime);
}

