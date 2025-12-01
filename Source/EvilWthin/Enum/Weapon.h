#pragma once

UENUM(BlueprintType)
enum class EDetectiveWeapons : uint8
{
	REVOLVER = 0 	UMETA(DisplayName = "REVOLVER"),
	SHOTGUN 		UMETA(DisplayName = "SHOTGUN"),
	BOLT			UMETA(DisplayName = "BOLT"),
	RIFLE   		UMETA(DisplayName = "RIFLE"),
	MAG 			UMETA(DisplayName = "MAG"),
};
