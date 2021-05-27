/**
 * Unreal Version: 5.0.0
 * Template: First Person
 * Project Name: FPUE5
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MyTriggerBox.generated.h"

UCLASS()
class FPUE5_API AMyTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

private:
	// constructor
	AMyTriggerBox();

	/** 
	* This function can be called whatever you want.
	* Since this going to be used as the dynamic function for OnActorBeginOverlap, it is mandatory that takes in two AActor* parameters
	* AActor's OnActorBeginOverlap signature can be seen on line 78 at /Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h
	*/
	UFUNCTION()
	void BeginOverlap(class AActor* OverlappedActor, class AActor* OtherActor);
};
