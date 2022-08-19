// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "MProjectileBase.h"
#include "MAttributeComponent.h"
#include "MInteractComponent.h"
#include "UEtestGameModeBase.h"
#include "Action/MActionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMCharacter::AMCharacter():bIsAlive(true), HitTimeParam("TimeOnHit")
{

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractComp = CreateDefaultSubobject<UMInteractComponent>("InteractComp");

	AttributeComp = CreateDefaultSubobject<UMAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<UMActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

FVector AMCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void AMCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddUniqueDynamic(this, &AMCharacter::OnHealthChanged);
	AttributeComp->OnActorKilled.BindUObject(GetWorld()->GetAuthGameMode<AUEtestGameModeBase>(), &AUEtestGameModeBase::OnPlayerKilled);
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
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void AMCharacter::BlackholeAttack()
{
	ActionComp->StartActionByName(this, "Blackhole");
}

void AMCharacter::Teleport(){
	ActionComp->StartActionByName(this, "Teleport");
}

void AMCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AMCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void AMCharacter::OnHealthChanged(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (!bIsAlive)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, GetNameSafe(this) + TEXT("死亡后仍受到伤害"));
		return;
	}
	if(Delta<0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(HitTimeParam, GetWorld()->TimeSeconds);
	}
	if(NewHealth<=0.0f)
	{
		bIsAlive = false;
		DisableInput(Cast<APlayerController>(GetController()));
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();//关闭后死亡会立刻静止
		
		//GetCapsuleComponent()->SetEnableGravity(false);
		//如果人物死亡时仍在运动，则关闭碰撞会造成穿模
		//SetActorEnableCollision(false);
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
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMCharacter::SprintStop);
}

bool AMCharacter::HealSelf(float HealAmount)
{
	if (HealAmount)return AttributeComp->ApplyHealthChange(this, HealAmount);
	else return AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax());
}
