// Fill out your copyright notice in the Description page of Project Settings.


#include "MExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AMExplosiveBarrel::AMExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetSimulatePhysics(true);  //��������ģ��
	StaticMeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	StaticMeshComp->SetCollisionProfileName("PhysicsActor");

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(StaticMeshComp);
	RadialForceComp->ImpulseStrength = 2500.0f;
	RadialForceComp->bImpulseVelChange = true; //ʹ������������������
	RadialForceComp->Radius = 750.0f;
	RadialForceComp->Falloff = RIF_Linear;//������˥��
	RadialForceComp->SetAutoActivate(false);//������أ���ôһ��ʼ�ͻ��о���������������
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	//StaticMeshComp->OnComponentHit.AddUniqueDynamic(this, &AMExplosiveBarrel::OnCompHit);
}

// Called when the game starts or when spawned
void AMExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComp->OnComponentHit.AddUniqueDynamic(this, &AMExplosiveBarrel::OnCompHit);
}

void AMExplosiveBarrel::OnCompHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                                  UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "boom!");
	RadialForceComp->FireImpulse();
}

// Called every frame
void AMExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

