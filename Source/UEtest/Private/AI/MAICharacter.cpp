// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MAICharacter.h"

#include "AIController.h"
#include "MAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AMAICharacter::AMAICharacter():HandSocket("Muzzle_01"), HitTimeParam("TimeOnHit"), HealthThreshold(0.4f)
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UMAttributeComponent>("AttributeComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

int AMAICharacter::GetBotNum()
{
	return ExistBotNum;
}


void AMAICharacter::OnSeePawn(APawn* Pawn)
{
	if(SetTargetActor(Pawn))
	{
		DrawDebugString(GetWorld(), Pawn->GetActorLocation(), "OnSeePawn", nullptr, FColor::Blue, 3.0f, true, 1);
	}
}

void AMAICharacter::OnHealthChanged(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(Delta<0.0f)
	{
		if(InstigatorActor!=this)
		{
			SetTargetActor(InstigatorActor);
		}
		GetMesh()->SetScalarParameterValueOnMaterials(HitTimeParam, GetWorld()->TimeSeconds);
	}
	if(NewHealth<=0.0f)
	{
		//SetActorEnableCollision(false);
		DetachFromControllerPendingDestroy();
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll", false);
		SetLifeSpan(5.0f);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Red, OwningComp->IsAlive() ? "AI死亡BUG" : "AI Killed");
	}
	else 
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if(AIController)
		{
			UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
			if (NewHealth / OwningComp->GetHealthMax() <= HealthThreshold)
			{
				BlackboardComp->SetValueAsBool(IsLowHealth, true);
			}
			else BlackboardComp->SetValueAsBool(IsLowHealth, false);
		}
		
	}
}

bool AMAICharacter::SetTargetActor(AActor* TargetActor)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	//未被AIControl控制时不应执行
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TargetActorName, TargetActor);
		return true;
	}
	return false;
}

void AMAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddUniqueDynamic(this, &AMAICharacter::OnSeePawn);
	AttributeComp->OnHealthChanged.AddUniqueDynamic(this, &AMAICharacter::OnHealthChanged);
}

void AMAICharacter::BeginPlay()
{
	Super::BeginPlay();
	++ExistBotNum;
}

void AMAICharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	--ExistBotNum;
}

