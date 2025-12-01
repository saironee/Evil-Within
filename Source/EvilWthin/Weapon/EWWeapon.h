// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/EWAttackInterface.h"
#include "Enum/Weapon.h"
#include "InputActionValue.h"
#include "Interface/EWFireInterface.h"
#include "Interface/EWReloadInterface.h"
#include "Interface/EWReloadCheck.h"

#include "EWWeapon.generated.h"

UCLASS()
class EVILWTHIN_API AEWWeapon : public AActor, public IEWFireInterface, public IEWReloadInterface, public IEWReloadCheck
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEWWeapon();

protected:
	ACharacter* DetectiveCharacter;

protected:
	virtual void Tick(float DeltaTime) override;
	
protected:
	void SetGun(EDetectiveWeapons NewWeapon);

public:
	void AttachGun(ACharacter* TargetCharacter);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataAsset")
	TObjectPtr<class UEWWeaponDataAssets> GunDataAsset;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataAsset")
	TArray<class UEWWeaponDataAssets*> WeaponsList;
	
protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> GunBody;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	EDetectiveWeapons CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TArray<uint8> AmmoList;

//Input Section	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> IASwap1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> IASwap2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> IASwap3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> IMCGun;

protected:	
	virtual void fire() override;

	virtual void Reload() override;

	virtual void ReloadCheck() override;
	
	void ReleaseFire();

	void SwapRevolver();
	void SwapShotgun();

public:

	EDetectiveWeapons GetCurrentWeapon() { return CurrentWeapon; }

	uint8 bIsFire : 1 = false;

	uint8 bIsSwap : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunLock")
	uint8 bCanRevolver : 1 = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunLock")
	uint8 bCanShotgun : 1 = false;
	
protected:
	uint8 bCanFire : 1 = true;

	uint8 bCanFireRelease : 1 = false;
	
//CoolTime	
protected:
	FTimerHandle HFireTimer;

	FTimerHandle HReloadTimer;

	FTimerHandle HSwapTimer;
};
