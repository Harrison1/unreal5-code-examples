// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHoldingSidearmAnim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHoldingRifleAnim = false;

protected:

	UFUNCTION(BlueprintCallable)
	void AttachPistol();

	UFUNCTION(BlueprintCallable)
	void AttachRifle();

};
