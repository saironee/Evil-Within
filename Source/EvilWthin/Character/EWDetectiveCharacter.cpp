// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/EWDetectiveCharacter.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MovieSceneTracksComponentTypes.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "DataAsset/EWDetectiveDataAsset.h"

#include "Kismet/GameplayStatics.h"

#include "Weapon/EWWeaponDataAssets.h"
#include "DataAsset/EWCharacterDataAsset.h"

#include "Weapon/EWWeapon.h"


void AEWDetectiveCharacter::Test()
{
	
}

AEWDetectiveCharacter::AEWDetectiveCharacter()
{
	//Mesh Setting
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Detective/Detective.Detective'"));
	if(BodyRef.Succeeded())
		GetMesh()->SetSkeletalMesh(BodyRef.Object);

	//InputSetting
	static ConstructorHelpers::FObjectFinder<UInputAction> IAMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputAction/IA_Movement.IA_Movement'"));
	if(IAMoveRef.Succeeded())
		IAMove = IAMoveRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IALookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputAction/IA_Look.IA_Look'"));
	if(IALookRef.Succeeded())
		IALook = IALookRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IARunRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputAction/IA_Run.IA_Run'"));
	if(IARunRef.Succeeded())
		IARun = IARunRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IAAimRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputAction/IA_Aim.IA_Aim'"));
	if(IAAimRef.Succeeded())
		IAAim = IAAimRef.Object;

	
	static ConstructorHelpers::FObjectFinder<UInputAction> IAReloadRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputAction/IA_Reload.IA_Reload'"));
	if(IAReloadRef.Succeeded())
		IAReload = IAReloadRef.Object;

	//CameraSetting
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if(SpringArm != nullptr)
		SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if(Camera != nullptr)
		Camera->SetupAttachment(SpringArm);

	//Stat Setting
	ConstructorHelpers::FObjectFinder<UEWDetectiveDataAsset> MoveStatRef(TEXT("/Script/EvilWthin.EWDetectiveDataAsset'/Game/DataAsset/Detective/DA_DetectiveMove.DA_DetectiveMove'"));
	if(MoveStatRef.Succeeded())
		MoveStat = MoveStatRef.Object;
	
	ConstructorHelpers::FObjectFinder<UEWDetectiveDataAsset> AimStatRef(TEXT("/Script/EvilWthin.EWDetectiveDataAsset'/Game/DataAsset/Detective/DA_DetectiveAim.DA_DetectiveAim'"));
	if(AimStatRef.Succeeded())
		AimStat = AimStatRef.Object;

	ConstructorHelpers::FObjectFinder<UEWDetectiveDataAsset> ReloadStatRef(TEXT("/Script/EvilWthin.EWDetectiveDataAsset'/Game/DataAsset/Detective/DA_DetectiveReload.DA_DetectiveReload'"));
	if(ReloadStatRef.Succeeded())
		ReloadStat = ReloadStatRef.Object;

	ConstructorHelpers::FObjectFinder<UEWCharacterDataAsset> DetectiveStatRef(TEXT("/Script/EvilWthin.EWCharacterDataAsset'/Game/DataAsset/Detective/DA_DetectiveStat.DA_DetectiveStat'"));
	if(DetectiveStatRef.Succeeded())
		DetectiveStat = DetectiveStatRef.Object;
}

void AEWDetectiveCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetCurrenStat(EDetectiveStat::MOVE);
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager;
	if(CameraManager)
	{
		CameraManager->ViewPitchMax = 35.f;
		CameraManager->ViewPitchMin = -35.f;
	}

	DefaultArmLength = MoveStat->TargetArmLength;
	AimArmLength = AimStat->TargetArmLength;

	CurrentHealth = DetectiveStat->MaxHealth;

	Gun = GetWorld()->SpawnActor<AEWWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	Gun->AttachGun(this);
}

void AEWDetectiveCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Aim(DeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("%f"), GetVelocity().Size());
}

void AEWDetectiveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhandInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if(EnhandInputComponent)
	{
		EnhandInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &AEWDetectiveCharacter::Move);
		EnhandInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &AEWDetectiveCharacter::Look);
		EnhandInputComponent->BindAction(IARun, ETriggerEvent::Triggered, this, &AEWDetectiveCharacter::RunTrigger);
		EnhandInputComponent->BindAction(IARun, ETriggerEvent::Completed, this, &AEWDetectiveCharacter::RunRelease);
		EnhandInputComponent->BindAction(IAAim, ETriggerEvent::Triggered, this, &AEWDetectiveCharacter::AimTrigger);
		EnhandInputComponent->BindAction(IAAim, ETriggerEvent::Completed, this, &AEWDetectiveCharacter::AimRelease);
		EnhandInputComponent->BindAction(IAReload, ETriggerEvent::Triggered, this, &AEWDetectiveCharacter::Reload);
		EnhandInputComponent->BindAction(IAReload, ETriggerEvent::Completed, this, &AEWDetectiveCharacter::ReloadComplete);
	}
}

void AEWDetectiveCharacter::Move(const FInputActionValue& Value)
{
	if(bCanMove)
	{
		SetCurrenStat(EDetectiveStat::MOVE);
	
		FVector2D MoveVector = Value.Get<FVector2D>();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MoveVector.X);
		AddMovementInput(RightDirection, MoveVector.Y);
	}
}

void AEWDetectiveCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void AEWDetectiveCharacter::AimTrigger(const FInputActionValue& Value)
{
	if(bCanAim && !Gun)
		return;
	
	bCanRun = false;
	bIsAiming = true;
	
	SetCurrenStat(EDetectiveStat::AIM);
}

void AEWDetectiveCharacter::AimRelease(const FInputActionValue& Value)
{
	bCanAim = true;
	
	bCanRun = true;
	bIsAiming = false;

	if(CurrentStat == EDetectiveStat::AIM)
		SetCurrenStat(EDetectiveStat::MOVE);
	else
	{
		SetCurrenStat(CurrentStat);
	}
}

void AEWDetectiveCharacter::Aim(float DeltaTime)
{
	float TargetLength = bIsAiming ? AimArmLength : DefaultArmLength;
	SpringArm->TargetArmLength = FMath::Lerp(SpringArm->TargetArmLength, TargetLength, DeltaTime * AimTransitionSpeed);
}

void AEWDetectiveCharacter::Reload()
{
	if(bCanReload)
	{
		SetCurrenStat(EDetectiveStat::RELOAD);

		bCanAim = false;
		bIsAiming = false;

		bCanReload = false;
	}
}

void AEWDetectiveCharacter::ReloadOff()
{
	bCanAim = true;
	
	SetCurrenStat(EDetectiveStat::MOVE);
}

void AEWDetectiveCharacter::ReloadComplete()
{
	bCanReload = true;
}


void AEWDetectiveCharacter::SetCurrenStat(const EDetectiveStat NewStat)
{
	CurrentStat = NewStat;
	
	switch(CurrentStat)
	{
	case EDetectiveStat::MOVE:
		GetCharacterMovement()->bOrientRotationToMovement = MoveStat->bOrientRotationToMovement;
		bUseControllerRotationYaw = MoveStat->bUseControllerRotationYaw;
		bUseControllerRotationPitch = MoveStat->bUseControllerRotationPitch;
		bUseControllerRotationRoll = MoveStat->bUseControllerRotationRoll;

		GetCharacterMovement()->RotationRate = MoveStat->RotationRate;
		
		SpringArm->bInheritPitch = MoveStat->bInheritPitch;
		SpringArm->SetRelativeLocation(MoveStat->SpringArmLocation);

		GetCharacterMovement()->MaxWalkSpeed = MoveStat->MaxWalkSpeed;
		break;
	case EDetectiveStat::AIM:
		GetCharacterMovement()->bOrientRotationToMovement = AimStat->bOrientRotationToMovement;
		bUseControllerRotationYaw = AimStat->bUseControllerRotationYaw;
		bUseControllerRotationPitch = AimStat->bUseControllerRotationPitch;
		bUseControllerRotationRoll = AimStat->bUseControllerRotationRoll;

		GetCharacterMovement()->RotationRate = AimStat->RotationRate;

		SpringArm->SetRelativeLocation(AimStat->SpringArmLocation);
		SpringArm->bInheritPitch = AimStat->bInheritPitch;
		
		GetCharacterMovement()->MaxWalkSpeed = AimStat->MaxWalkSpeed;
		break;
	case EDetectiveStat::RELOAD:
		GetCharacterMovement()->bOrientRotationToMovement = ReloadStat->bOrientRotationToMovement;
		bUseControllerRotationYaw = ReloadStat->bUseControllerRotationYaw;
		bUseControllerRotationPitch = ReloadStat->bUseControllerRotationPitch;
		bUseControllerRotationRoll = ReloadStat->bUseControllerRotationRoll;

		GetCharacterMovement()->RotationRate = ReloadStat->RotationRate;
		
		SpringArm->bInheritPitch = ReloadStat->bInheritPitch;
		SpringArm->SetRelativeLocation(ReloadStat->SpringArmLocation);

		GetCharacterMovement()->MaxWalkSpeed = ReloadStat->MaxWalkSpeed;
		break;
	default:
		break;
	}
}

void AEWDetectiveCharacter::Dead()
{
	Super::Dead();
	
	SetCurrenStat(EDetectiveStat::DIE);
}