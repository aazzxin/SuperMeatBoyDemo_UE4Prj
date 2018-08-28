// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SuperMeatBoyCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "LevelManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// ASuperMeatBoyCharacter

ASuperMeatBoyCharacter::ASuperMeatBoyCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	
	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<UClampSpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 3150.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	MoveComp = GetCharacterMovement();
	// 设定角色
	JumpMaxCount = 2;
	SavedState = State = ECharacterState::Idle;

	
}

void ASuperMeatBoyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<ASuperMeatBoyGameMode>(GetWorld()->GetAuthGameMode());
}

// 检测是否抓到墙体
bool ASuperMeatBoyCharacter::CheckIsGrab()
{
	FHitResult hit(ForceInit);
	FVector start, end, horizon,vertical;
	start = MoveComp->GetActorLocation();
	
	end = start + FVector(InputComponent->GetAxisValue("MoveRight") * 50, 0, 0);
	GetWorld()->LineTraceSingleByObjectType(
		hit,
		start,
		end,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner()));
	//DrawDebugLine(this->GetWorld(), start, end, FColor::Black, false, 0, 0, 10);

	if (hit.GetActor() && GetVelocity().Z < GrabVelocityThreshold) {
		if (GrabActor == nullptr || hit.GetActor() != GrabActor) {
			GrabActor = hit.GetActor();
			SavedState = State;
		}
		State = ECharacterState::Grab;
		GetSprite()->SetLooping(false);
	}
	else {
		State = SavedState;
	}
	switch (State) {
	case ECharacterState::Grab :
		GetCharacterMovement()->Velocity.Z += GrabUpForce;
		//UE_LOG(LogTemp, Warning, TEXT("Current State:Grab"));
		break;
	case ECharacterState::Walk :
		//UE_LOG(LogTemp, Warning, TEXT("Current State:Walk"));
		break;
	case ECharacterState::Idle:
		//UE_LOG(LogTemp, Warning, TEXT("Current State:Idle"));
		break;
	case ECharacterState::GrabJump:
		//UE_LOG(LogTemp, Warning, TEXT("Current State:GrabJump"));
		break;
	case ECharacterState::Die:
		//UE_LOG(LogTemp, Warning, TEXT("Current State:Die"));
		break;
	default:
		//UE_LOG(LogTemp, Warning, TEXT("Current State:Other"));
		break;
	}
	
	return hit.GetActor();
}

void ASuperMeatBoyCharacter::SetDied()
{
	State = ECharacterState::Die;
	GetSprite()->SetLooping(false);
	//GetOwner()->SetActorLocation(GameMode->GetCheckPos());
    //GetWorld()->GetAuthGameMode()
}

void ASuperMeatBoyCharacter::SetCameraBound()
{
	FIntPoint Point = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();
	float DstWidth = SideViewCameraComponent->OrthoWidth / 2;
	float DstHeight = DstWidth * Point.Y / Point.X * 1.08f;
	CameraBoom->LevelBoundXMin = LevelBoundXMin - 0.5 * TileWidth + DstWidth;
	CameraBoom->LevelBoundXMax = LevelBoundXMax - DstWidth;
	CameraBoom->LevelBoundZMin = LevelBoundZMin + DstHeight;
	CameraBoom->LevelBoundZMax = LevelBoundZMax - DstHeight;
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ASuperMeatBoyCharacter::UpdateAnimation()
{
	UPaperFlipbook* DesiredAnimation;
	switch (State) {
	case ECharacterState::Grab:
		DesiredAnimation = GrabAnimation;
		break;
	case ECharacterState::JumpUp:
	case ECharacterState::GrabJump:
		DesiredAnimation = JumpAnimation;
		break;
	case ECharacterState::Die:
		DesiredAnimation = DieAnimation;
		/*UE_LOG(LogTemp, Error, TEXT("Anim Length: %d"), GetSprite()->GetFlipbookLengthInFrames()-1);
		UE_LOG(LogTemp, Error, TEXT("Current Length: %d"), GetSprite()->GetPlaybackPositionInFrames());
		UE_LOG(LogTemp, Error, TEXT("Current State: %s"), *GetSprite()->GetFlipbook()->GetName());*/
		break;
	default:
		const FVector PlayerVelocity = GetVelocity();
		const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

		// Are we moving or standing still?
		DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
		break;
	}

	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
		GetSprite()->Play();
	}
}

void ASuperMeatBoyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// 限制角色移动的左右范围
	if (GameMode->GetState() == GameModeState::Play) {
		SetActorLocation(FVector(FMath::Clamp<float>(GetActorLocation().X, LevelBoundXMin, LevelBoundXMax), GetActorLocation().Y, GetActorLocation().Z));
		if (GetActorLocation().Z < LevelBoundZMin)
			ReBorn(GameMode->GetCheckPos());
		UpdateCharacter();
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void ASuperMeatBoyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASuperMeatBoyCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASuperMeatBoyCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASuperMeatBoyCharacter::TouchStopped);
	PlayerInputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &ASuperMeatBoyCharacter::EscapeAction);
}

void ASuperMeatBoyCharacter::ReBorn(FVector Pos)
{
	//State = ECharacterState::Idle;
	TeleportTo(Pos,FRotator::ZeroRotator);
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
}

bool ASuperMeatBoyCharacter::CanJumpInternal_Implementation() const
{
	bool bCanJump = !bIsCrouched;
	const bool bCanHoldToJumpHigher = (GetJumpMaxHoldTime() > 0.0f) && IsJumpProvidingForce();
	bCanJump &= MoveComp && MoveComp->IsJumpAllowed() && !MoveComp->bWantsToCrouch && JumpCurrentCount < JumpMaxCount;

	return bCanJump;
}

void ASuperMeatBoyCharacter::Jump()
{
	if (State == ECharacterState::Die)
		return;
	bIsLanded = false;
	GetSprite()->SetLooping(false);
	if (State == ECharacterState::Grab) {
		State = ECharacterState::GrabJump;
		MoveComp->Velocity.Z = GrabJumpVelocityZ;
		MoveComp->Velocity.X = -GrabJumpVelocityX * Face;
		return;
	}
	else if(CanJumpInternal_Implementation())
		State = ECharacterState::JumpUp;
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
}

void ASuperMeatBoyCharacter::StopJumping()
{
	bPressedJump = false;
	ResetJumpState();
}

void ASuperMeatBoyCharacter::Landed(const FHitResult & Hit)
{
	bIsLanded = true;
	if (State == ECharacterState::Die) 
		return;
	if (Hit.GetActor()) {
		SavedState = State = GetVelocity().X < 1.0f ? ECharacterState::Idle : ECharacterState::Walk;
	}
	GetSprite()->SetLooping(true);
}

void ASuperMeatBoyCharacter::EscapeAction()
{
	LevelBoundXMax = LevelBoundXMin = LevelBoundZMax = LevelBoundZMin = 0;
	CameraBoom->LevelBoundXMin = CameraBoom->LevelBoundXMax = CameraBoom->LevelBoundZMin = CameraBoom->LevelBoundZMax = 0;
	//GameMode->SwitchState(GameModeState::LevelSelect);
	GameMode->EscapeScene();
}

void ASuperMeatBoyCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/
	if (State == ECharacterState::Die) {
		if (GetSprite()->GetFlipbook() == DieAnimation && GetSprite()->GetPlaybackPositionInFrames() == GetSprite()->GetFlipbookLengthInFrames() - 1) {
			if (bIsLanded || GetActorLocation().X<LevelBoundXMin || GetActorLocation().X>LevelBoundXMax || GetActorLocation().Z > LevelBoundZMax) {
				State = ECharacterState::Idle;
				TeleportTo(GameMode->GetCheckPos(), FRotator::ZeroRotator);
			}
		}
		return;
	}
	CheckIsGrab();

	if (State == ECharacterState::GrabJump)
		return;
	FVector Loc = GetActorLocation();
    // Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void ASuperMeatBoyCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void ASuperMeatBoyCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void ASuperMeatBoyCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();


	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 0.0f, 0.0f));
			Face = -1;
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 180.0f, 0.0f));
			Face = 1;
		}
	}
}
