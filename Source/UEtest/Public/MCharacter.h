// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UETEST_API AMCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;

public:
	// Sets default values for this character's properties
	AMCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float X);
	void MoveRight(float X);
	void PrimaryAttack();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
