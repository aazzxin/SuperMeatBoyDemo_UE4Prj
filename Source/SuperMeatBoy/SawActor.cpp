// Fill out your copyright notice in the Description page of Project Settings.

#include "SawActor.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

ASawActor::ASawActor() {

}

void ASawActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAnimation();
}

void ASawActor::BeginPlay()
{
	Super::BeginPlay();
	CurrentRotator = GetActorRotation();
}

void ASawActor::OverlapAction(ASuperMeatBoyCharacter* Character)
{
	FVector Dist = Character->GetActorLocation();
	Dist -= GetActorLocation();
	if (Character) {
		UE_LOG(LogTemp, Error, TEXT("Character Set Died!"));
		Character->GetCharacterMovement()->Velocity += Dist * ImpactSpeed;
		Character->SetDied();
	}
}

void ASawActor::UpdateAnimation()
{
	CurrentRotator.Pitch += GetWorld()->DeltaTimeSeconds * RotateSpeed;
	if (CurrentRotator.Pitch > 360)
		CurrentRotator.Pitch -= 360;
	SetActorRotation(CurrentRotator);
}
