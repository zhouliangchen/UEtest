// Fill out your copyright notice in the Description page of Project Settings.


#include "MItemChest.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMItemChest::AMItemChest():RotationPitch(110.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChestMesh");
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(ChestMesh);
	//第一步，为对象启用网络复制
	//SetReplicates(true);
	bReplicates = true;
}

void AMItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void AMItemChest::OnRep_LidOpened()
{
	LidMesh->SetRelativeRotation(FRotator(bLidOpened ? RotationPitch : 0.0f, 0, 0));
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange, "Chest has been opened!");
}

void AMItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//第三步，为要复制的属性指定复制的规则
	DOREPLIFETIME(AMItemChest, bLidOpened);
}



// Called every frame
void AMItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

