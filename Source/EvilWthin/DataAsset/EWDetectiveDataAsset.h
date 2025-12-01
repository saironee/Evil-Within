// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "EWDetectiveDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class EVILWTHIN_API UEWDetectiveDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ControllerSetting)
	uint8 bOrientRotationToMovement : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ControllerSetting)
	uint8 bUseControllerRotationYaw : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ControllerSetting)
	uint8 bUseControllerRotationPitch : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ControllerSetting)
	uint8 bUseControllerRotationRoll : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ControllerSetting)
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ControllerSetting)
	uint8 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ControllerSetting)
	uint8 bInheritRoll : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ControllerSetting)
	uint8 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ControllerSetting)
	uint8 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSetting)
	FVector SpringArmLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSetting)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaxWalkSpeed)
	float MaxWalkSpeed;
};
