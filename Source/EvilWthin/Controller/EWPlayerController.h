// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EWPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EVILWTHIN_API AEWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEWPlayerController();

public:
	virtual void BeginPlay() override;

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> IMCDefault;
};
