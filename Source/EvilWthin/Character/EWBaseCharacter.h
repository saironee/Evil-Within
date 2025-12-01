// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/EWAttackInterface.h"

#include "EWBaseCharacter.generated.h"

UCLASS()
class EVILWTHIN_API AEWBaseCharacter : public ACharacter, public IEWAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEWBaseCharacter();

protected:
	virtual void BeginPlay() override;

public:
	uint8 bCanHit : 1 = true;
	
	uint8 bIsHit : 1 = false;

protected:
	float CurrentHealth;

protected:
	virtual void TakeDamage(float Damage) override;
	
	virtual void Dead();

protected:
	FTimerHandle HHitTimer;
};	