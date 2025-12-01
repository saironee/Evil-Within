// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EWBaseCharacter.h"
#include <InputActionValue.h>

#include "GameFramework/CharacterMovementComponent.h"

#include "Enum/DetectiveStat.h"
#include "Weapon/EWWeapon.h"

#include "Interface/EWReloadOffInterface.h"
#include "Interface/EWReloadInterface.h"

#include "EWDetectiveCharacter.generated.h"

/**
 * 
 */
UCLASS()
class EVILWTHIN_API AEWDetectiveCharacter : public AEWBaseCharacter,
public IEWReloadOffInterface,
public IEWReloadInterface
{
	GENERATED_BODY()

protected:
	void Test();

public:
	AEWDetectiveCharacter();

public:
	virtual void BeginPlay() override;

protected:
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

//InputSection
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> IALook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> IARun;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAAim;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAReload;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void RunTrigger(const FInputActionValue& Value)
	{
		if(bCanRun)
			GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	void RunRelease(const FInputActionValue& Value)
	{
		if(bCanRun)
			SetCurrenStat(EDetectiveStat::MOVE);
	}

	void AimTrigger(const FInputActionValue& Value);
	void AimRelease(const FInputActionValue& Value);

	void Aim(float DeltaTime);

	virtual void Reload() override;

	virtual void ReloadOff() override;

	void ReloadComplete();
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim")
	uint8 bIsAiming : 1 = false;

protected:
	uint8 bCanRun : 1 = true;

	uint8 bCanAim : 1 = true;
	
	uint8 bCanReload : 1 = true;

	uint8 bCanMove : 1 = true;

	uint8 bCanAttack : 1 = true;

	float DefaultArmLength;
	float AimArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float AimTransitionSpeed = 5.0f;
	
	FTimerHandle FAttackTimer;

	FTimerHandle FAimTimer;

	FTimerHandle FReloadTimer;

	FTimerHandle FPunchTimer;

	FTimerHandle FPunchCoolTimer;
//CameraSection	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class UCameraComponent> Camera;

//Stat Section
protected:
	void SetCurrenStat(const EDetectiveStat NewStat);

public:
	EDetectiveStat GetCurrenStat() { return CurrentStat; }

	UCameraComponent* GetCameraComponent() { return Camera; }
protected:
	EDetectiveStat CurrentStat;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<class UEWDetectiveDataAsset> MoveStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<class UEWDetectiveDataAsset> AimStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<class UEWDetectiveDataAsset> ReloadStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<class UEWCharacterDataAsset> DetectiveStat;

//Gun Section
protected:
	virtual void Dead() override;
	
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<AEWWeapon> Gun;
};
