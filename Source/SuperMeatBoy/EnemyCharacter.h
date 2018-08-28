// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Components/ShapeComponent.h"
#include "SuperMeatBoyCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API AEnemyCharacter : public APaperCharacter
{
	GENERATED_BODY()


	AEnemyCharacter();

	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = Behavior)
		class UBehaviorTree* EnemyBehavior;

protected:

	UPROPERTY(EditAnywhere, Category = Render)
	UShapeComponent * Collision;
	UShapeComponent * StableCollision;

	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnimation;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* DieAnimation;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	void UpdateCharacter();

	FScriptDelegate OverlapBeginDelegate;
	FScriptDelegate OverlapEndDelegate;
	
	virtual void OverlapBeginAction(ASuperMeatBoyCharacter* Character);// 一般触碰
	virtual void OverlapEndAction(ASuperMeatBoyCharacter* Character);
	virtual void OverlapTopAction(ASuperMeatBoyCharacter* Character);// 顶部触碰

	virtual void SetDied();
	
	ECharacterState State;
    float ImpactDump = 1100.0f; // 角色踩下跳起的影响值

private:
	UFUNCTION()
	void OverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool AnimReady;
};
