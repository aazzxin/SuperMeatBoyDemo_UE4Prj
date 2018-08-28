// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

		AEnemyAIController(const FObjectInitializer& ObjectInitializer);

public:
	virtual void BeginPlay() override;
	virtual void Possess(class APawn* InPawn) override;
	virtual void UnPossess() override;


	void SwitchTarget();

protected:
	virtual void Tick(float DeltaTime) override;

	/*int TargetLocationKeyID;
	int SelfLocationKeyID;
	int CurDirectionKeyID;*/
	int LeftLocationKeyID;
	int RightLocationKeyID;
	int TargetLocationKeyID;
	int DistanceKeyID;


private:
	UPROPERTY(transient)
	UBlackboardComponent* BlackboardComp;
	UPROPERTY(transient)
	 UBehaviorTreeComponent* BehaviorComp;

	class AEnemyCharacter* Enemy;
	APawn* TargetPawn;

	FVector LeftVector, RightVector, TargetVector;
};
