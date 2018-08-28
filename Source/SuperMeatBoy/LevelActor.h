// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SuperMeatBoyCharacter.h"
#include "Components/ShapeComponent.h"
#include "LevelActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelActorOverlapAct);

UCLASS()
class SUPERMEATBOY_API ALevelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Render)
	UShapeComponent* Collision;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* Sprite;

	virtual void OverlapAction(ASuperMeatBoyCharacter* Character);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FLevelActorOverlapAct OverlapCallback;
	
private:
	UFUNCTION()
	void OnOverlapBegin(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	FScriptDelegate OverlapDelegate;
};
