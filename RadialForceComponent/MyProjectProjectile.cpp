// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponentPool.h"
#include "PhysicsEngine/RadialForceComponent.h"

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
	if (Explosion->IsValid())
	{
		// spawn explosion on hit
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Explosion, GetActorLocation(), FRotator(1.0f), FVector(2.0f), true, true, ENCPoolMethod::AutoRelease, true);

		// credit goes to https://answers.unrealengine.com/questions/354174/making-a-force-burst-in-c.html
		URadialForceComponent* ForceComp = NewObject<URadialForceComponent>(this, TEXT("Radial Force Component"));
		ForceComp->RegisterComponent();
		ForceComp->bImpulseVelChange = 1;
		ForceComp->SetWorldLocation(GetActorLocation());
		ForceComp->SetWorldRotation(GetActorRotation());

		ForceComp->Radius = 500;
		ForceComp->ImpulseStrength = 2500.0f;
		ForceComp->Falloff = ERadialImpulseFalloff::RIF_Linear;
		ForceComp->FireImpulse();
	}

	Destroy();
}
