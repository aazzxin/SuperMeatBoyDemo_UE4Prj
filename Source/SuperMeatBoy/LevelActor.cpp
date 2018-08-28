// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelActor.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/World.h"

// Sets default values
ALevelActor::ALevelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ALevelActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (Collision) {
		UE_LOG(LogTemp, Warning, TEXT("Collosion CheckIn"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Collosion Genearte"));
		Collision = FindComponentByClass<UShapeComponent>();
	}
	OverlapDelegate.BindUFunction(this, FName("OnOverlapBegin"));
	Collision->OnComponentBeginOverlap.Add(OverlapDelegate);
}

void ALevelActor::OverlapAction(ASuperMeatBoyCharacter* Character)
{

}

// Called every frame
void ALevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelActor::OnOverlapBegin(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Collosion Overlapping!"));
	if (Collision->IsOverlappingActor(GetWorld()->GetFirstPlayerController()->GetPawn()))
		OverlapAction(Cast<ASuperMeatBoyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()));
}

