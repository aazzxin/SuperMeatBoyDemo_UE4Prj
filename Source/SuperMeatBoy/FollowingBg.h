// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "FollowingBg.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API AFollowingBg : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Sprite", AllowPrivateAccess = "true"))
		class UPaperSpriteComponent* RenderComponent;
public:
	AFollowingBg();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// AActor interface
#if WITH_EDITOR
	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
#endif
	// End of AActor interface

	/** Returns RenderComponent subobject **/
	FORCEINLINE class UPaperSpriteComponent* GetRenderComponent() const { return RenderComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	AActor * FollowingTarget;
	UCameraComponent* Camera;
};
