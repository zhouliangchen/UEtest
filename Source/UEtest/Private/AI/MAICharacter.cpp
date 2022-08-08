// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MAICharacter.h"

#include "Perception/PawnSensingComponent.h"

// Sets default values
AMAICharacter::AMAICharacter()
{
	UPawnSensingComponent* PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
}

void AMAICharacter::OnSeePawn(APawn* Pawn)
{

}

void AMAICharacter::PostInitializeComponents()
{
	PawnSensingComp->OnSeePawn.AddUniqueDynamic(this, &AMAICharacter::OnSeePawn);
}

