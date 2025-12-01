#include "Weapon/EWWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/EWWeaponDataAssets.h"
#include "Character/EWDetectiveCharacter.h"
#include "Interface/EWAttackInterface.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"
#include "Interface/EWReloadOffInterface.h"

AEWWeapon::AEWWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//Setup
	GunBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunBody"));
	SetRootComponent(GunBody);

	ConstructorHelpers::FObjectFinder<UEWWeaponDataAssets> RevolverRef(TEXT("/Script/EvilWthin.EWWeaponDataAssets'/Game/Weapons/WeaponsData/RevolverData.RevolverData'"));
	if (RevolverRef.Succeeded())
	{
		WeaponsList.Add(RevolverRef.Object);
	}

	ConstructorHelpers::FObjectFinder<UEWWeaponDataAssets> ShotgunRef(TEXT("/Script/EvilWthin.EWWeaponDataAssets'/Game/Weapons/WeaponsData/ShotGunData.ShotGunData'"));
	if (ShotgunRef.Succeeded())
	{
		WeaponsList.Add(ShotgunRef.Object);
	}

	//Input
	ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCGunRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Gun.IMC_Gun'"));
	if (IMCGunRef.Succeeded())
	{
		IMCGun = IMCGunRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> IAFireRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputAction/IA_Fire.IA_Fire'"));
	if (IAFireRef.Succeeded())
	{
		IAFire = IAFireRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> IASwap1Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputAction/IA_SwapWeapon1.IA_SwapWeapon1'"));
	if (IASwap1Ref.Succeeded())
	{
		IASwap1 = IASwap1Ref.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> IASwap2Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputAction/IA_SwapWeapon2.IA_SwapWeapon2'"));
	if (IASwap2Ref.Succeeded())
	{
		IASwap2 = IASwap2Ref.Object;
	}
	
	CurrentWeapon = EDetectiveWeapons::REVOLVER;
	
	AmmoList.Add(WeaponsList[static_cast<int32>(EDetectiveWeapons::REVOLVER)]->MaxAmmo);
	AmmoList.Add(WeaponsList[static_cast<int32>(EDetectiveWeapons::SHOTGUN)]->MaxAmmo);
}

void AEWWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("%d"), AmmoList[static_cast<int32>(CurrentWeapon)]);
}

void AEWWeapon::SetGun(EDetectiveWeapons NewWeapon)
{
	CurrentWeapon = NewWeapon;

	if (WeaponsList.IsValidIndex(static_cast<int32>(CurrentWeapon)))
	{
		GunBody->SetSkeletalMesh(WeaponsList[static_cast<int32>(CurrentWeapon)]->Mesh);
		SetActorRelativeTransform(WeaponsList[static_cast<int32>(CurrentWeapon)]->Transform);
	}
}

void AEWWeapon::AttachGun(ACharacter* TargetCharacter)
{
	DetectiveCharacter = TargetCharacter;
	
	SetOwner(DetectiveCharacter);
	
	AttachToComponent(DetectiveCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("GunSocket")));
	
	if(DetectiveCharacter)
	{
		
		APlayerController* PlayerController = Cast<APlayerController>(DetectiveCharacter->GetController());
		if (PlayerController)
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if (Subsystem)
			{
				Subsystem->AddMappingContext(IMCGun, 1);
			}
		
			UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
			if (EnhancedInputComponent)
			{
				EnhancedInputComponent->BindAction(IAFire, ETriggerEvent::Triggered, this, &AEWWeapon::fire);
				EnhancedInputComponent->BindAction(IAFire, ETriggerEvent::Completed, this, &AEWWeapon::ReleaseFire);
				EnhancedInputComponent->BindAction(IASwap1, ETriggerEvent::Completed, this, &AEWWeapon::SwapRevolver);
				EnhancedInputComponent->BindAction(IASwap2, ETriggerEvent::Completed, this, &AEWWeapon::SwapShotgun);
			}

		}	
	}

	SetGun(EDetectiveWeapons::REVOLVER);
}

void AEWWeapon::fire()
{
	if(!bCanFire)
		return;
		
	FHitResult HitResult;

	AEWDetectiveCharacter* MyDetective = Cast<AEWDetectiveCharacter>(DetectiveCharacter);

	if (MyDetective) {
		if (MyDetective->bIsAiming && !bIsFire) {

			if(AmmoList[static_cast<int32>(CurrentWeapon)] > 0)
				AmmoList[static_cast<int32>(CurrentWeapon)]--;
			else
			{
				bCanFire = false;
				if(IEWReloadInterface* ReloadActor = Cast<IEWReloadInterface>(DetectiveCharacter))
				{
					ReloadActor->Reload();
				}
				return;
			}
			
			bIsFire = true;
			bCanFire = false;
			bCanFireRelease = false;

			GetWorld()->GetTimerManager().SetTimer(HFireTimer, FTimerDelegate::CreateLambda([this]
			{
				bIsFire = false;
				bCanFireRelease = true;
			}), WeaponsList[static_cast<int32>(CurrentWeapon)]->fireSpeed, false);

			UCameraComponent* DetectiveCamera = MyDetective->GetCameraComponent();
			if (DetectiveCamera)
			{
				FVector TraceStart = DetectiveCamera->GetComponentLocation();
				FVector TraceEnd = TraceStart + (DetectiveCamera->GetForwardVector() * WeaponsList[static_cast<int32>(CurrentWeapon)]->Distance);

				float BulletRadius = WeaponsList[static_cast<int32>(CurrentWeapon)]->BulletRadius;
				
				FCollisionShape SphereShape = FCollisionShape::MakeSphere(BulletRadius);
				
				bool bIsHit = GetWorld()->LineTraceSingleByChannel(
					HitResult,
					TraceStart,
					TraceEnd,
					ECC_GameTraceChannel1
				);

				if (bIsHit)
				{
					IEWAttackInterface* HitActor = Cast<IEWAttackInterface>(HitResult.GetActor());
					if (HitActor)
					{
						HitActor->TakeDamage(WeaponsList[static_cast<int32>(CurrentWeapon)]->Damage);
					}
				}
			}
		}
	}
}

void AEWWeapon::Reload()
{
	bCanFire = true;
	
	if(AmmoList[static_cast<int32>(CurrentWeapon)] + 1 < WeaponsList[static_cast<int32>(CurrentWeapon)]->MaxAmmo)
	{
		++AmmoList[static_cast<int32>(CurrentWeapon)];
	}else
	{
		++AmmoList[static_cast<int32>(CurrentWeapon)];
		if(IEWReloadOffInterface* ReloadActor = Cast<IEWReloadOffInterface>(DetectiveCharacter))
		{
			ReloadActor->ReloadOff();
		}
	}
}

void AEWWeapon::ReloadCheck()
{
	if(AmmoList[static_cast<int32>(CurrentWeapon)] >= WeaponsList[static_cast<int32>(CurrentWeapon)]->MaxAmmo)
	{
		
		if(IEWReloadOffInterface* ReloadActor = Cast<IEWReloadOffInterface>(DetectiveCharacter))
		{
			AmmoList[static_cast<int32>(CurrentWeapon)] = WeaponsList[static_cast<int32>(CurrentWeapon)]->MaxAmmo;
			ReloadActor->ReloadOff();
		}
	}
}

void AEWWeapon::ReleaseFire()
{
	if(bCanFireRelease)
		bCanFire = true;
}

void AEWWeapon::SwapRevolver()
{
	if(CurrentWeapon == EDetectiveWeapons::REVOLVER || !bCanRevolver)
		return;
	
	SetGun(EDetectiveWeapons::REVOLVER);

	bIsSwap = true;

	GetWorld()->GetTimerManager().SetTimer(HSwapTimer, FTimerDelegate::CreateLambda([this]{
		bIsSwap = false;
	}),0.2f, false);
}

//You naver with lose controll oh no that maen who the man sold the world

void AEWWeapon::SwapShotgun()
{
	if(CurrentWeapon == EDetectiveWeapons::SHOTGUN || !bCanShotgun)
		return;
	
	SetGun(EDetectiveWeapons::SHOTGUN);

	bIsSwap = true;

	GetWorld()->GetTimerManager().SetTimer(HSwapTimer, FTimerDelegate::CreateLambda([this]{
		bIsSwap = false;
	}),0.2f, false);
}
