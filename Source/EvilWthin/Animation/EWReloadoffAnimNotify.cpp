// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/EWReloadoffAnimNotify.h"
#include "Interface/EWReloadOffInterface.h"

void UEWReloadoffAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if(MeshComp)
	{
		if(IEWReloadOffInterface* ReloadActor = Cast<IEWReloadOffInterface>(MeshComp->GetOwner()))
		{
			ReloadActor->ReloadOff();
		}
	}
}
