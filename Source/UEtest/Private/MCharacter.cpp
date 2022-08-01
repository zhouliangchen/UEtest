// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "MInteractComponent.h"
#include "MMagicProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMCharacter::AMCharacter():AttackDelay(0.2f)
{

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractComp = CreateDefaultSubobject<UMInteractComponent>("InteractComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}


void AMCharacter::MoveForward(float X)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.0f;
	ControlRot.Pitch = 0.0f;
	AddMovementInput(ControlRot.Vector(), X);
}
void AMCharacter::MoveRight(float X)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.0f;
	ControlRot.Pitch = 0.0f;
	AddMovementInput(FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y), X);
}

void AMCharacter::PrimaryAttack()
{
	PlayAnimMontage(AnimAttack);
	GetWorldTimerManager().SetTimer(PrimaryAttackTimeHandle, this, &AMCharacter::PrimaryAttack_delay, AttackDelay, false);
}

void AMCharacter::PrimaryAttack_delay()
{
	SpawnProjectile(ProjectileClass);
}

void AMCharacter::BlackholeAttack()
{
	PlayAnimMontage(AnimAttack);
	GetWorldTimerManager().SetTimer(BlackholeTimeHandle, this, &AMCharacter::BlackholeAttack_delay, AttackDelay);
}

void AMCharacter::BlackholeAttack_delay(){
	SpawnProjectile(BlackholeClass);
}

void AMCharacter::Teleport(){
	PlayAnimMontage(AnimAttack);
	GetWorldTimerManager().SetTimer(TeleportTimeHandle, this, &AMCharacter::Teleport_delay, AttackDelay);
}

void AMCharacter::Teleport_delay()
{
	SpawnProjectile(TeleportClass);
}

void AMCharacter::SpawnProjectile(TSubclassOf<AMProjectileBase> Projectile)
{
	if(ensureAlways(Projectile))
	{
		FVector CameraLocation = CameraComp->GetComponentLocation();
		FVector End = CameraLocation + GetControlRotation().Vector() * 10000;
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, End, ECC_WorldDynamic, Params);
		FVector TargetPoint = bHit ? HitResult.ImpactPoint : End;
		FVector RhandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		FVector SubVector = TargetPoint - RhandLocation;
		//SubVector.Rotation()
		//FRotator AimRotator(SubVector.X, SubVector.Y, SubVector.Z);
		FTransform SpawnTM(SubVector.Rotation(), RhandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		GetWorld()->SpawnActor<AActor>(Projectile, SpawnTM, SpawnParams);

		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, GetNameSafe(Projectile) + TEXT("已发射"));
	}
}

// Called to bind functionality to input
void AMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AMCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, InteractComp, &UMInteractComponent::PrimaryInteract);
	PlayerInputComponent->BindAction("BlackholeAttack", IE_Pressed, this, &AMCharacter::BlackholeAttack);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &AMCharacter::Teleport);
}

