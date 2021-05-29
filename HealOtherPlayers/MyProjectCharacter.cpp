// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	HealingSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealingSphere"));
	HealingSphere->SetGenerateOverlapEvents(true);
	HealingSphere->SetWorldScale3D(FVector(0.0f));
	HealingSphere->BodyInstance.SetCollisionProfileName("NoCollision");
	HealingSphere->SetupAttachment(RootComponent);
}

void AMyProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	/** 
	* Set up overlaps for the sphere component
	* put in begin play. credit to https://answers.unrealengine.com/questions/804022/c-oncomponentbeginoverlap-not-working.html (good thread on the topic)
	* Doesn't work in cnostructor for some reason. I don't know why. This might work if you delete the Blueprint from the editor and then add it back to the viewport. I don't know.
	*/
	HealingSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCharacter::OnHealBeginOverlap);
	HealingSphere->OnComponentEndOverlap.AddDynamic(this, &AMyProjectCharacter::OnHealEndOverlap);

	BaseMaterialDynamic = GetMesh()->CreateDynamicMaterialInstance(0, GetMesh()->GetMaterial(0));
	GetMesh()->SetMaterial(0, BaseMaterialDynamic);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &AMyProjectCharacter::OpenHealingSphere);
	PlayerInputComponent->BindAction("Heal", IE_Released, this, &AMyProjectCharacter::CloseHealingSphere);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyProjectCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyProjectCharacter::OnResetVR);
}


void AMyProjectCharacter::OnResetVR()
{
	// If MyProject is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in MyProject.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMyProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMyProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMyProjectCharacter::OpenHealingSphere()
{
	Heal();

	// scale healing sphere to overlap overlap characters
	if (HealingSphere != nullptr)
	{
		HealingSphere->SetWorldScale3D(FVector(10.0f));
	}
}

void AMyProjectCharacter::CloseHealingSphere()
{
	StopHealing();

	// close healing sphere
	if (HealingSphere != nullptr)
	{
		HealingSphere->SetWorldScale3D(FVector(0.0f));
	}
}

void AMyProjectCharacter::Heal()
{
	// change the materials's Healing scalar paramter in the dynamic material to a float higher than 0.0
	if (BaseMaterialDynamic != nullptr)
	{
		BaseMaterialDynamic->SetScalarParameterValue("Healing Param", 2.0f);
	}

	// perform game logic here
}

void AMyProjectCharacter::StopHealing()
{
	// change the materials's Healing scalar paramter in the dynamic material to something less than 0.0
	if (BaseMaterialDynamic != nullptr)
	{
		BaseMaterialDynamic->SetScalarParameterValue("Healing Param", -2.0f);
	}

	// end game logic here

}

void AMyProjectCharacter::OnHealBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent != nullptr && OtherActor != nullptr && OtherActor != this)
	{
		// credit to https://answers.unrealengine.com/questions/49076/how-to-use-isa-to-check-is-an-actor-is-of-a-class.html
		if (OtherActor->IsA(AMyProjectCharacter::StaticClass()))
		{
			// heal characters in sphere by running the character's Heal function
			// your have to first cast to AMyProjectCharacter to run the character's function. 
			AMyProjectCharacter* Teammate = Cast<AMyProjectCharacter>(OtherActor);
			Teammate->Heal();
		}
	}
}

void AMyProjectCharacter::OnHealEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedComponent != nullptr && OtherActor != nullptr && OtherActor != this)
	{
		if (OtherActor->IsA(AMyProjectCharacter::StaticClass()))
		{
			// end healing when character exits the sphere or when the sphere closes
			AMyProjectCharacter* Teammate = Cast<AMyProjectCharacter>(OtherActor);
			Teammate->StopHealing();
		}
	}
}
