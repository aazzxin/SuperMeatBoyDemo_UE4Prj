// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SwitchTarget.h"
#include "EnemyAIController.h"

EBTNodeResult::Type UBTTask_SwitchTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AEnemyAIController* MyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (MyController == NULL)
	{
		return EBTNodeResult::Failed;
	}
	MyController->SwitchTarget();
	return EBTNodeResult::Succeeded;
}
