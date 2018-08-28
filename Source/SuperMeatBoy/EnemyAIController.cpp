// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	BrainComponent = BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BrainComp"));
	
	bWantsPlayerState = true;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	Enemy = Cast<AEnemyCharacter>(GetPawn());
	if (Enemy) {
		if (Enemy->EnemyBehavior->BlackboardAsset) {
			//BlackboardComp->InitializeBlackboard(*Enemy->EnemyBehavior->BlackboardAsset);
			UseBlackboard(Enemy->EnemyBehavior->BlackboardAsset, BlackboardComp);
		}
		UE_LOG(LogTemp, Warning, TEXT("Run Tree"));
		/*TargetLocationKeyID = BlackboardComp->GetKeyID("TargetLocation");
		SelfLocationKeyID = BlackboardComp->GetKeyID("SelfLocation");
		CurDirectionKeyID = BlackboardComp->GetKeyID("CurDirection");*/
		LeftLocationKeyID = BlackboardComp->GetKeyID("LeftLocation");
		RightLocationKeyID = BlackboardComp->GetKeyID("RightLocation");
		TargetLocationKeyID = BlackboardComp->GetKeyID("TargetLocation");
		DistanceKeyID = BlackboardComp->GetKeyID("Distance");
		TargetVector = LeftVector = Enemy->GetActorLocation() + FVector(-600, 0, 0);
		RightVector = Enemy->GetActorLocation();
		BlackboardComp->SetValue<UBlackboardKeyType_Vector>(LeftLocationKeyID, LeftVector);
		BlackboardComp->SetValue<UBlackboardKeyType_Vector>(RightLocationKeyID, RightVector);
		BlackboardComp->SetValue<UBlackboardKeyType_Vector>(TargetLocationKeyID, TargetVector);
		BlackboardComp->SetValue<UBlackboardKeyType_Float>(DistanceKeyID, 0.5f);
		FVector LeftLocation = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(LeftLocationKeyID);
		FVector RightLocation = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(RightLocationKeyID);

		UE_LOG(LogTemp, Warning, TEXT("LeftLocation:%s - RightLocation:%s"), *LeftLocation.ToString(), *RightLocation.ToString());
		//BehaviorComp->StartTree(*(Enemy->EnemyBehavior));
		RunBehaviorTree(Enemy->EnemyBehavior);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Posses"));
	//Enemy = Cast<AEnemyCharacter>(InPawn);

	//if (Enemy && Enemy->EnemyBehavior) {
	//	if (Enemy->EnemyBehavior->BlackboardAsset) {
	//		//BlackboardComp->InitializeBlackboard(*Enemy->EnemyBehavior->BlackboardAsset);
	//		UseBlackboard(Enemy->EnemyBehavior->BlackboardAsset, BlackboardComp);
	//	}

	//	UE_LOG(LogTemp, Warning, TEXT("Run Tree"));
	//	TargetLocationKeyID = BlackboardComp->GetKeyID("TargetLocation");
	//	SelfLocationKeyID = BlackboardComp->GetKeyID("SelfLocation");
	//	CurDirectionKeyID = BlackboardComp->GetKeyID("CurDirection");
	//	LeftLocationKeyID = BlackboardComp->GetKeyID("LeftLocation");
	//	RightLocationKeyID = BlackboardComp->GetKeyID("RightLocation");
	//	BlackboardComp->SetValue<UBlackboardKeyType_Vector>(LeftLocationKeyID, InPawn->GetActorLocation() + FVector(-20, 0, 0));
	//	BlackboardComp->SetValue<UBlackboardKeyType_Vector>(RightLocationKeyID, InPawn->GetActorLocation());
	//	FVector LeftLocation = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(LeftLocationKeyID);

	//	UE_LOG(LogTemp, Warning, TEXT("LeftLocation:%s"),*LeftLocation.ToString());
	//	//BehaviorComp->StartTree(*(Enemy->EnemyBehavior));
	//	RunBehaviorTree(Enemy->EnemyBehavior);
	//}
}

void AEnemyAIController::UnPossess()
{
	Super::UnPossess();

	UE_LOG(LogTemp, Warning, TEXT("UnPossess"));
	BehaviorComp->StopTree();
}

void AEnemyAIController::Tick(float DeltaTime)
{
	//SetParameters();
}

void AEnemyAIController::SwitchTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("Switch"));
	TargetVector = TargetVector == LeftVector ? RightVector : LeftVector;
	BlackboardComp->SetValue<UBlackboardKeyType_Vector>(TargetLocationKeyID, TargetVector);
}

