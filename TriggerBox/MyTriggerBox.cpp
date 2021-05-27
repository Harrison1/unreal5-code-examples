/** 
* Unreal Version: 5.0.0
* Template: First Person
* Project Name: FPUE5
* Trigger Box Documentation Link : https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Engine/ATriggerBox/
* A Trigger Box is a child of an Actor and each Actor has a dynamic OnActorBeginOverlap function: https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/GameFramework/AActor/
*/

#include "MyTriggerBox.h"

AMyTriggerBox::AMyTriggerBox()
{
	// each AActor has the OnActorBeginOverlap dynamic function
	OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBox::BeginOverlap);
}

void AMyTriggerBox::BeginOverlap(class AActor* OverlappedActor, class AActor* OtherActor)
{
	// https://answers.unrealengine.com/questions/419732/print-to-screen-using-c.html
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Begin Overlap Action Message"));
	}
}
