// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckNear.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API UBTDecorator_CheckNear : public UBTDecorator
{
	GENERATED_BODY()

	UBTDecorator_CheckNear(const FObjectInitializer& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:

	UPROPERTY(EditAnywhere, Category = Condition)
		struct FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category = Condition)
		struct FBlackboardKeySelector DistanceKey;

private:
	bool LOSTrace(AActor* InActor, const FVector& EndLocation,float Distance) const;
	
};
