// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPickupWeapon.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UDataTable;
class URotatingMovementComponent;

UCLASS()
class MYPROJECT_API AMyPickupWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPickupWeapon();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Table)
	UDataTable* WeaponTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Table)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URotatingMovementComponent* RotatingComponent;


	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult& SweepResult
	);
};
