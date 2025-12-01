// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/EWRevolverReloadNotify.h"
#include "Character/EWDetectiveCharacter.h"
#include "Interface/EWReloadInterface.h"

void UEWRevolverReloadNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp)
	{
		if(AEWDetectiveCharacter* DetectiveCharacter = Cast<AEWDetectiveCharacter>(MeshComp->GetOwner()))
		{
			if(IEWReloadInterface* GunActor = Cast<IEWReloadInterface>(DetectiveCharacter->Gun))
			{
				GunActor->Reload();
			}
		}
		
	}
}
