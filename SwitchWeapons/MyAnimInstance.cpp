// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyProjectCharacter.h"

void UMyAnimInstance::AttachPistol()
{
	AMyProjectCharacter* Player = Cast<AMyProjectCharacter>(TryGetPawnOwner());

	if (Player)
	{
		if (bHoldingRifleAnim)
		{
			bHoldingRifleAnim = false;
			Player->Rifle->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("spine_rifle")));
		}
		FName PistolSocket = bHoldingSidearmAnim ? FName(TEXT("hand_r_pistol")) : FName(TEXT("spine_pistol"));
		Player->Pistol->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, PistolSocket);
	}
}

void UMyAnimInstance::AttachRifle()
{
	AMyProjectCharacter* Player = Cast<AMyProjectCharacter>(TryGetPawnOwner());

	if (Player)
	{
		if (bHoldingSidearmAnim)
		{
			bHoldingSidearmAnim = false;
			Player->Pistol->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("spine_pistol")));
		}
		FName RifleSocket = bHoldingRifleAnim ? FName(TEXT("hand_r_rifle")) : FName(TEXT("spine_rifle"));
		Player->Rifle->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RifleSocket);
	}
}
