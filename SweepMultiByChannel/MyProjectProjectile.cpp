// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponentPool.h"

AMyProjectProjectile::AMyProjectProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMyProjectProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AMyProjectProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		// play explosion sound on impact
		if (ExplosionSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
		}


		// Spawn Niagara explosion
		if (SimpleExplosion != nullptr)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SimpleExplosion, GetActorLocation(), FRotator(0.0f), FVector(1), true, true, ENCPoolMethod::AutoRelease, true);
		}

		FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(500.0f);

		// create a new hit array everytime. This array will be populated when we run GetWorld()->SweepMultiByChannel
		TArray<FHitResult> HitArray;

		// ignore self. credit to https://forums.unrealengine.com/t/line-tracing-help-thank-you/6178
		FCollisionQueryParams TraceParams(FName(TEXT("Explosion Trace")), false, this);

		bool ContainsBlockingHits = GetWorld()->SweepMultiByChannel(HitArray, GetActorLocation(), GetActorLocation(), GetActorRotation().Quaternion(), ECC_PhysicsBody, ExplosionSphere, TraceParams);

		if (ContainsBlockingHits)
		{
			// loop through TArray
			for (FHitResult& Hit : HitArray)
			{

				if (Hit.GetComponent()->IsSimulatingPhysics())
				{
					// some useful debug messages that might help
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit Result Name: %s"), *Hit.GetActor()->GetName()));
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Impact Normal: %s"), *Hit.Location.ToString()));
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Impact Normal: %s"), *Hit.ImpactNormal.ToString()));
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Impact Point: %s"), *Hit.ImpactPoint.ToString()));
					}

					Hit.GetComponent()->AddImpulseAtLocation(-Hit.ImpactNormal * 1500.0f, Hit.Location);
				}
			}
		}

		Destroy();
	}
}
