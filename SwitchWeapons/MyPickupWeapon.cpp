// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPickupWeapon.h"
#include "Components/SphereComponent.h"
#include "MyProjectCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DataTable.h"
#include "MyWeapon.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
AMyPickupWeapon::AMyPickupWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	CollisionComponent->InitSphereRadius(100.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComponent->SetGenerateOverlapEvents(true);
	RootComponent = CollisionComponent;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement Component"));
	RotatingComponent->RotationRate = FRotator(0.f, 45.f, 0.f);
}

// Called when the game starts or when spawned
void AMyPickupWeapon::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyPickupWeapon::OnOverlapBegin);

	if (WeaponTable)
	{
		FWeaponData* WeaponData = WeaponTable->FindRow<FWeaponData>(RowName, "");

		if (WeaponData->Mesh != nullptr)
		{
			WeaponMesh->SetSkeletalMesh(WeaponData->Mesh);
		}
	}

}

void AMyPickupWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OverlappedComp != nullptr && OtherActor != this && OtherComp != nullptr)
	{

		AMyProjectCharacter* Player = Cast<AMyProjectCharacter>(OtherActor);

		if (Player)
		{

			FWeaponData* WeaponData = WeaponTable->FindRow<FWeaponData>(RowName, "");

			if (WeaponData)
			{

				AMyWeapon* NewWeapon = NewObject<AMyWeapon>();

				if (NewWeapon)
				{
					NewWeapon->WeaponType = WeaponData->WeaponType;
					NewWeapon->Damage = WeaponData->Damage;
				}
				
				if (WeaponData->WeaponType == EWeaponType::Pistol)
				{
					Player->Pistol->SetSkeletalMesh(WeaponData->Mesh);
					Player->MyWeapons.Emplace("Pistol", NewWeapon);
				}

				if (WeaponData->WeaponType == EWeaponType::Rifle)
				{
					Player->Rifle->SetSkeletalMesh(WeaponData->Mesh);
					Player->MyWeapons.Emplace("Rifle", NewWeapon);
				}

				Destroy();
			}
		}
	}
}
