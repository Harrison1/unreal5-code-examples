// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "MyWeapon.generated.h"


class USkeletalMesh;

/** 
	Pistol	UMETA(DisplayName = "Pistol"),
	Rifle	UMETA(DisplayName = "Rifle")
*/
UENUM(BlueprintType)
enum class EWeaponType : uint8 
{
	Pistol,
	Rifle
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int32 RateOfFire;
};

UCLASS()
class MYPROJECT_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

public:	

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere)
	int32 Damage;

	UPROPERTY(EditAnywhere)
	FName Name;
};
