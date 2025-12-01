// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EWPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AEWPlayerController::AEWPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCDefaultRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if(IMCDefaultRef.Succeeded())
		IMCDefault = IMCDefaultRef.Object;
}

void AEWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(IMCDefault, 0);
	}
}