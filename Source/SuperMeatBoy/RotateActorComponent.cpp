// Fill out your copyright notice in the Description page of Project Settings.

#include "RotateActorComponent.h"


// Sets default values for this component's properties
URotateActorComponent::URotateActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URotateActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Actor = GetOwner();
	TargetActor = Actor->GetAttachParentActor();
	if (TargetActor)
		Rotator = TargetActor->GetActorRotation();
}


void URotateActorComponent::SetAnimation(float DeltaTime)
{
	if (TargetActor) {
		Rotator.Pitch += Speed * DeltaTime;
		if (Rotator.Pitch > 360)
			Rotator.Pitch -= 360;
		TargetActor->SetActorRotation(Rotator);
	}
}

// Called every frame
void URotateActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	SetAnimation(DeltaTime);
}

