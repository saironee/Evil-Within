#pragma once

UENUM(BlueprintType)
enum class EDetectiveStat : uint8
{
	MOVE 		UMETA(DisplayName = "MOVE"),
	AIM 		UMETA(DisplayName = "AIM"),
	RELOAD		UMETA(DisplayName = "RELOAD"),
	DIE 		UMETA(DisplayName = "DIE"),
	PUNCH		UMETA(DisplayName = "PUNCH"),
};