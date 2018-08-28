// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SuperMeatBoyGameMode.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "PaperTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "ClampSpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "SuperMeatBoyCharacter.generated.h"

class UTextRenderComponent;



// 人物状态
UENUM(BlueprintType)
enum class ECharacterState :uint8 {
	Idle,
	Walk,
	JumpUp,//跳起
	Fall, //下落
	Grab, //抓墙
	GrabJump, //抓墙跳，此时不调用基类跳跃，而是自己添加速度。且此时无法左右移动
	Die
};
/**
 * This class is the default character for SuperMeatBoy, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class ASuperMeatBoyCharacter : public APaperCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UClampSpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;
protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* JumpAnimation;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* GrabAnimation;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* DieAnimation;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


	// 重写动作操作
	bool CanJumpInternal_Implementation() const override;
	void Jump() override;
	void StopJumping() override;
	void Landed(const FHitResult &Hit) override;
	//void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	void EscapeAction();

public:
	ASuperMeatBoyCharacter();
	virtual void BeginPlay() override;

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	bool CheckIsGrab();

	void SetDied();
	void SetCameraBound();
	void ReBorn(FVector Pos);
	FORCEINLINE void SetMobility(bool Mobility) { GetCapsuleComponent()->Mobility = Mobility ? EComponentMobility::Movable : EComponentMobility::Static; }
	FORCEINLINE void SetCameraWidth(float Width) { SideViewCameraComponent->OrthoWidth = Width; }
	FORCEINLINE void SetCheckPoint(FVector Pos) { GameMode->SetCheckPos(Pos); }

	float LevelBoundXMin, LevelBoundXMax;
	float LevelBoundZMin, LevelBoundZMax;
	float TileWidth = 58.0f;// 2DTile地图的格子宽度

private:
	ASuperMeatBoyGameMode* GameMode;

	UCharacterMovementComponent * MoveComp;

	AActor * GrabActor;// 抓取的墙面
	ECharacterState SavedState; //抓取前的状态
	ECharacterState State;

	/*UPROPERTY(EditAnywhere, Category = LavelMap)
	APaperTileMapActor * Map;*/

	int Face = 1;// 朝向
	bool bIsLanded;// 是否落地
	const float GrabVelocityThreshold = 5.0f;//纵轴速度小于多少时，视为抓取墙面
	const float GrabJumpVelocityZ = 1000.0f;// 抓墙跳时的上升速度
	const float GrabJumpVelocityX = 800.0f;// 抓墙跳时的反弹速度
	const float GrabUpForce = 5.0f;// 抓取墙面时给予的上升力

};
