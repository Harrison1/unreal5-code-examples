// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyProjectCharacter.generated.h"

class USkeletalMeshComponent;
class AMyWeapon;

UCLASS(config=Game)
class AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMyProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;


	/** AnimMontage to play when the character equips or switches the sidearm */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages)
	UAnimMontage* EquipSidearmMontage;

	/** AnimMontage to play when the character equips or switches the rifle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages)
	UAnimMontage* EquipRifleMontage;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapons)
	// TArray<int32> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapons)
	USkeletalMeshComponent* Pistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapons)
	USkeletalMeshComponent* Rifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapons)
	TMap<FString, AMyWeapon*> MyWeapons;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/** Equip Pistol */
	void EquipSidearm();

	/** Equip Rifle */
	void EquipRifle();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
