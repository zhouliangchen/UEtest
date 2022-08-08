// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class UETEST_API AMAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMAICharacter();

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);
	virtual void PostInitializeComponents()override;

};
