// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Translate.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API UBTTaskNode_Translate : public UBTTaskNode
{
	GENERATED_BODY()

		UBTTaskNode_Translate(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = Condition)
	struct FBlackboardKeySelector TargetKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
