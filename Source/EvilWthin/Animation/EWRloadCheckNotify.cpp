// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/EWRloadCheckNotify.h"
#include "Interface/EWReloadCheck.h"
#include "Character/EWDetectiveCharacter.h"
#include "Weapon/EWWeapon.h"

void UEWRloadCheckNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp)
	{
		if(AEWDetectiveCharacter* DetectiveCharacter = Cast<AEWDetectiveCharacter>(MeshComp->GetOwner()))
		{
			if(AEWWeapon* Gun = DetectiveCharacter->Gun)
			{
				if(IEWReloadCheck* ReloadActor = Cast<IEWReloadCheck>(Gun))
				{
					ReloadActor->ReloadCheck();
				}
			}
		}
	}
}
