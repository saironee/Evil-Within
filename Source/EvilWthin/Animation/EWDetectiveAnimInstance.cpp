// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/EWDetectiveAnimInstance.h"

#include "Character/EWDetectiveCharacter.h"
#include <GameFramework/Character.h>
#include "GameFramework/CharacterMovementComponent.h"

#include "Weapon/EWWeapon.h"

#include "Animation/AnimSequence.h"
#include "Animation/BlendSpace1D.h"


UEWDetectiveAnimInstance::UEWDetectiveAnimInstance()
{
	
}

void UEWDetectiveAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerPawn = Cast<ACharacter>(GetOwningActor());
	if(OwnerPawn)
	{
		Movement = OwnerPawn->GetCharacterMovement();
		DetectiveCharacter = Cast<AEWDetectiveCharacter>(OwnerPawn);
	}
	
}

void UEWDetectiveAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (Movement)
	{
		MovementSpeed = Movement->Velocity.Size2D();
		
		float TempPitchAngle = OwnerPawn->GetControlRotation().Pitch;
		PitchAngle = FMath::UnwindDegrees(TempPitchAngle);  // FMath::UnwindDegrees 사용하면 -180~180으로 나오게 해줌 <- 개꿀딱

		bIsHit = DetectiveCharacter->bIsHit;
	}

	if (DetectiveCharacter)
	{
		CurrentDetectiveStat = DetectiveCharacter->GetCurrenStat();
		
		if(AEWWeapon* Gun = Cast<AEWWeapon>(DetectiveCharacter->Gun))
		{
			CurrentDetectiveWeapon = Gun->GetCurrentWeapon();
			bIsFire = Gun->bIsFire;
			bIsSwap = Gun->bIsSwap;
		}
		
	}
	
}
