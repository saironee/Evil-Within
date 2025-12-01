// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EWCharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class EVILWTHIN_API UEWCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UEWCharacterDataAsset();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentHealth;
};
