// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EWWeaponDataAssets.generated.h"

/**
 * 
 */
UCLASS()
class EVILWTHIN_API UEWWeaponDataAssets : public UDataAsset
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Distance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float BulletRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float fireSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float ReloadReadyTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FTransform Transform;
};
