// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EWReloadOffInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEWReloadOffInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EVILWTHIN_API IEWReloadOffInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ReloadOff() = 0;
};
