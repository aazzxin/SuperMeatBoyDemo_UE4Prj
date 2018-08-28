// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Engine/World.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter() {
	AIControllerClass = AEnemyAIController::StaticClass();
	
	
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCharacter();
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Collision = FindComponentByClass<UShapeComponent>();
	TArray<USceneComponent*> SceneComponents = Collision->GetAttachChildren();
	for (int i = 0; i < SceneComponents.Num(); i++) {
		if (SceneComponents[i]->IsA<UShapeComponent>()) {
			//UE_LOG(LogTemp, Error, TEXT("Check A Shape"));
			StableCollision = Cast<UShapeComponent>(SceneComponents[i]);
			break;
		}
	}

	if (Collision) {
		//UE_LOG(LogTemp, Error, TEXT("Find Collision"));
		/*Collision->OnComponentBeginOverlap.Add(OverlapBeginDelegate);
		Collision->OnComponentEndOverlap.Add(OverlapEndDelegate);*/
		Collision->OnComponentBeginOverlap.__Internal_AddDynamic(this, &AEnemyCharacter::OverlapBegin, TEXT("OverlapBegin"));
		Collision->OnComponentEndOverlap.__Internal_AddDynamic(this, &AEnemyCharacter::OverlapEnd, TEXT("OverlapEnd"));
	}
	AnimReady = RunningAnimation && IdleAnimation&&DieAnimation;
}

void AEnemyCharacter::UpdateAnimation()
{
	UPaperFlipbook* DesiredAnimation;
	switch (State) {
	case ECharacterState::Die:
		DesiredAnimation = DieAnimation;
		break;
	default:
		const FVector PlayerVelocity = GetVelocity();
		const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

		// Are we moving or standing still?
		DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
		break;
	}

	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
		GetSprite()->Play();
	}
}

void AEnemyCharacter::UpdateCharacter()
{
	if (AnimReady)
		// Update animation to match the motion
		UpdateAnimation();


	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 5.0f)
		{
			//Controller->SetControlRotation(FRotator(0.0, 0.0f, 0.0f));
			SetActorRotation(FRotator(0.0, 0.0f, 0.0f));
		}
		else if (TravelDirection > 5.0f)
		{
			//Controller->SetControlRotation(FRotator(0.0f, 180.0f, 0.0f));
			SetActorRotation(FRotator(0.0, 180.0f, 0.0f));
		}
	}
}

void AEnemyCharacter::OverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Overlap Begin"));
	if (Collision->IsOverlappingActor(GetWorld()->GetFirstPlayerController()->GetPawn())) {
		ASuperMeatBoyCharacter* Character = Cast<ASuperMeatBoyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		FVector Direction = SweepResult.Location - GetActorLocation();
		float Ratio = abs(Direction.Z / Direction.X);
		if (Ratio > 0.4f)
			OverlapTopAction(Character);
		else if (Ratio < 0.3f)
			OverlapBeginAction(Character);

	}
		
}

//void AEnemyCharacter::OverlapBegin(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
//{
//	UE_LOG(LogTemp, Error, TEXT("Overlap Begin"));
//	if (Collision->IsOverlappingActor(GetWorld()->GetFirstPlayerController()->GetPawn()))
//		OverlapBeginAction(Cast<ASuperMeatBoyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()));
//
//}

void AEnemyCharacter::OverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Error, TEXT("Overlap End"));
	if (OtherActor->IsA<ASuperMeatBoyCharacter>())
		OverlapEndAction(Cast<ASuperMeatBoyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()));
}

void AEnemyCharacter::OverlapBeginAction(ASuperMeatBoyCharacter * Character)
{
	//UE_LOG(LogTemp, Error, TEXT("Overlap Begin Action"));
	//Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	Character->SetDied();
}

void AEnemyCharacter::OverlapEndAction(ASuperMeatBoyCharacter * Character)
{
	//UE_LOG(LogTemp, Error, TEXT("Overlap End Action"));
	//Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
}

void AEnemyCharacter::OverlapTopAction(ASuperMeatBoyCharacter * Character)
{
	//UE_LOG(LogTemp, Error, TEXT("Overlap Top"));
	FVector Dist;
	Dist.Z = 1000;
	Dist.X = Character->GetVelocity().X;
	//FVector JumpVector = FVector(Character->GetVelocity().X, 0, ImpactDump);
	Character->GetCharacterMovement()->Velocity.Z = ImpactDump ;
	Character->JumpCurrentCount = 0;
	//SetDied();
}

void AEnemyCharacter::SetDied()
{
	State = ECharacterState::Die;
	Collision->OnComponentBeginOverlap.Clear();
	//Collision->OnComponentEndOverlap.Clear();
	GetSprite()->SetLooping(false);
	if (StableCollision)
		StableCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Controller->UnPossess();
}

