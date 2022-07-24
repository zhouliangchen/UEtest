// Fill out your copyright notice in the Description page of Project Settings.


#include "MItemChest.h"

// Sets default values
AMItemChest::AMItemChest():RotationPitch(110.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChestMesh");
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(ChestMesh);
}

void AMItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(RotationPitch, 0, 0));
}

// Called when the game starts or when spawned
void AMItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

