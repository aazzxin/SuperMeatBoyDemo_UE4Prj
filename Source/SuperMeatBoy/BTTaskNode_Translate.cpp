// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_Translate.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTaskNode_Translate::UBTTaskNode_Translate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Translate";
	// accept only actors and vectors	
	TargetKey.AddVectorFilter(this, *NodeName);
}

EBTNodeResult::Type UBTTaskNode_Translate::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("ExcuteTask"));
	AEnemyAIController* MyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (MyController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	APawn* MyPawn = MyController->GetPawn();
	if (MyPawn) {
		const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
		if (MyBlackboard) {
			auto MyID = MyBlackboard->GetKeyID(TargetKey.SelectedKeyName);
			FVector TargetLocation;
			TargetLocation = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(MyID);
			FVector Direction = TargetLocation.X - MyPawn->GetActorLocation().X > 0 ? FVector(1, 0, 0) : FVector(-1, 0, 0);
			
			//UE_LOG(LogTemp, Warning, TEXT("Movement"));
			MyPawn->AddMovementInput(Direction);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
