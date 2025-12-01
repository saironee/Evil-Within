// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EWBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AEWBaseCharacter::AEWBaseCharacter()
{
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
	GetCapsuleComponent()->SetCollisionProfileName("EWCapsul");
	GetMesh()->SetCollisionProfileName("EWMesh");
	
	//GetMesh()->SetSimulatePhysics(true);
	//CurrentHealth = CharacterDataAsset.M
}

void AEWBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEWBaseCharacter::TakeDamage(float Damage)
{
	if(!bCanHit)
		return;
	
	bIsHit = true;
	bCanHit = false;
	
	GetWorld()->GetTimerManager().SetTimer(HHitTimer, FTimerDelegate::CreateLambda([this]
	{
		bIsHit = false;
		bCanHit = true;
		
		GetCharacterMovement()->Velocity = FVector(0, 0, GetCharacterMovement()->Velocity.Z);
	}), 0.2f, false);
	
	CurrentHealth -= Damage;

	if(CurrentHealth <= 0)
	{
		Dead();
	}
}

void AEWBaseCharacter::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionProfileName("EWDead");
	
	if (GetController())
	{
		GetController()->UnPossess();
	}
}
