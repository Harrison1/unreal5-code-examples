// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class USoundBase;
class UNiagaraSystem;

UCLASS(config=Game)
class AMyProjectProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	AMyProjectProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	/** Sound to play when the projectile hits something */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
	USoundBase* ExplosionSound;

	/** Spawn a simple explosion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
	UNiagaraSystem* SimpleExplosion;
};
