// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_CheckNear.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"


UBTDecorator_CheckNear::UBTDecorator_CheckNear(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Check Near";
	// accept only actors and vectors	
	TargetKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
	TargetKey.AddVectorFilter(this, *NodeName);
	DistanceKey.AddFloatFilter(this, *NodeName);
}

bool UBTDecorator_CheckNear::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AEnemyAIController* MyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	bool bIsNear = false;

	//UE_LOG(LogTemp, Warning, TEXT("CalculateRawCondition"));
	if (MyController && MyBlackboard) {
		auto MyID = MyBlackboard->GetKeyID(TargetKey.SelectedKeyName);
		auto TargetKeyType = MyBlackboard->GetKeyType(MyID);

		FVector TargetLocation;
		bool bGotTarget = false;
		AActor* TargetActor = NULL;
		if (TargetKeyType == UBlackboardKeyType_Object::StaticClass()) {
			UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(MyID);
			TargetActor = Cast<AActor>(KeyValue);
			if (TargetActor)
			{
				TargetLocation = TargetActor->GetActorLocation();
				bGotTarget = true;
			}
		}
		else if (TargetKeyType == UBlackboardKeyType_Vector::StaticClass()) {
			TargetLocation = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(MyID);
			bGotTarget = true;
		}
		auto DistanceID = MyBlackboard->GetKeyID(DistanceKey.SelectedKeyName);
		float Distance = MyBlackboard->GetValue<UBlackboardKeyType_Float>(DistanceKey.SelectedKeyName);
		if (bGotTarget == true)
		{
			if (LOSTrace(MyController->GetPawn(), TargetLocation, Distance) == true)
			{
				bIsNear = true;
			}
			/*else
				MyController->SwitchTarget();*/
		}
	}

	return bIsNear;
}

bool UBTDecorator_CheckNear::LOSTrace(AActor * InActor, const FVector & EndLocation, float Distance) const
{
	bool bIsNear = false;
	if (InActor) {
		//UE_LOG(LogTemp, Warning, TEXT("Actor:%f - Target:%f - Distance:%f"), InActor->GetActorLocation().X, EndLocation.X, Distance);
		if (abs(InActor->GetActorLocation().X - EndLocation.X) < Distance) {
			//UE_LOG(LogTemp, Error, TEXT("Near"));
			bIsNear = true;
		}
	}

	return bIsNear;
}
