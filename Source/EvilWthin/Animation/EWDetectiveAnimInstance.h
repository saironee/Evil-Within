// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/BlendSpace1D.h"

#include "Enum/DetectiveStat.h"
#include "Enum/Weapon.h"

#include "EWDetectiveAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class EVILWTHIN_API UEWDetectiveAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UEWDetectiveAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	class AEWDetectiveCharacter* DetectiveCharacter;

	class ACharacter* OwnerPawn;

	class UCharacterMovementComponent* Movement;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stat")
	EDetectiveStat CurrentDetectiveStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stat")
	EDetectiveWeapons CurrentDetectiveWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Angle")
	float PitchAngle;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trigger")
	uint8 bIsFire : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trigger")
	uint8 bIsReload : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trigger")
	uint8 bIsSwap : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trigger")
	uint8 bIsHit : 1;
};