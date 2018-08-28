// Fill out your copyright notice in the Description page of Project Settings.

#include "FollowingBg.h"
#include "Engine/CollisionProfile.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Engine/World.h"

AFollowingBg::AFollowingBg()
{
	RenderComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RenderComponent"));
	RenderComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	//RenderComponent->Mobility = EComponentMobility::Static;

	RenderComponent->AttachTo(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
}

#if WITH_EDITOR
bool AFollowingBg::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	Super::GetReferencedContentObjects(Objects);

	if (UPaperSprite* SourceSprite = RenderComponent->GetSprite())
	{
		Objects.Add(SourceSprite);
	}
	return true;
}
#endif

void AFollowingBg::BeginPlay()
{
	Super::BeginPlay();
	FollowingTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
	Camera = FollowingTarget->FindComponentByClass<UCameraComponent>();
	
}
void AFollowingBg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Pos = Camera->GetComponentLocation();
	SetActorLocation(FVector(Pos.X, -500, Pos.Z));
}
