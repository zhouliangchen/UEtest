// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MAICharacter.h"
#include "AIController.h"
#include "MAttributeComponent.h"
#include "MCharacter.h"
#include "SAdvancedRotationInputBox.h"
#include "UEtestGameModeBase.h"
#include "Action/MAction.h"
#include "Action/MActionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "UI/MAttachedWidget.h"

extern TAutoConsoleVariable<bool> CVarDebugDraw;
// Sets default values
AMAICharacter::AMAICharacter():HandSocket("Muzzle_01"), HitTimeParam("TimeOnHit"), HealthThreshold(0.4f)
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UMAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<UMActionComponent>("ActionComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

int AMAICharacter::GetBotNum()
{
	return ExistBotNum;
}


void AMAICharacter::OnSeePawn(APawn* Pawn)
{
	if(UMAttributeComponent::IsActorAlive(Pawn)&&SetTargetActor(Pawn))
	{
		if(CVarDebugDraw.GetValueOnGameThread())
		{
			DrawDebugString(GetWorld(), Pawn->GetActorLocation(), "OnSeePawn", nullptr, FColor::Blue, 3.0f, true, 1);
		}
		if (Pawn->GetClass()->IsChildOf(AMCharacter::StaticClass()))
		{
			if (!SpotWidgetInstance && SpotWidgetClass)
			{
				SpotWidgetInstance = CreateWidget<UMAttachedWidget>(GetWorld(), SpotWidgetClass);
			}
			if (SpotWidgetInstance)
			{
				SpotWidgetInstance->AttachTo = this;
				if (!SpotWidgetInstance->IsInViewport())
				{
					SpotWidgetInstance->AddToViewport();
				}
			}
		}
	}
}

bool AMAICharacter::SetTargetActor(AActor* TargetActor)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	//未被AIControl控制时不应执行
	if (AIController)
	{
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		UObject* Obj = BlackboardComp->GetValueAsObject(TargetActorName);
		if(Obj!=TargetActor)
		{
			BlackboardComp->SetValueAsObject(TargetActorName, TargetActor);
			return true;
		}
	}
	return false;
}

void AMAICharacter::OnHealthChanged(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(NewHealth<=0.0f)
	{
		if(Delta < 0.0f)
		{
			//ActionComp->StopAllAction();
			//ActionComp->DestroyComponent();
			ActionComp->bAlive = false;
			GetMesh()->SetScalarParameterValueOnMaterials(HitTimeParam, GetWorld()->TimeSeconds);
			if (HealthBarWidgetInstance)
			{
				HealthBarWidgetInstance->RemoveFromParent();
				HealthBarWidgetInstance = nullptr;  //置空后才能正常GC
			}
			//SetActorEnableCollision(false);
			DetachFromControllerPendingDestroy();
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll", false);

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			SetLifeSpan(5.0f);
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Red, OwningComp->IsAlive() ? TEXT("错误的AI死亡") : TEXT("AI Killed"));
		}
		else if(Delta == 0.0f)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Red,  TEXT("检测到AI死亡后遭受攻击"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Red,  TEXT("检测到AI死亡后被治疗，可能为bug"));
		}
	}
	else 
	{
		if (Delta < 0.0f)
		{
			if (InstigatorActor != this && UMAttributeComponent::IsActorAlive(InstigatorActor))
			{
				SetTargetActor(InstigatorActor);
			}

			GetMesh()->SetScalarParameterValueOnMaterials(HitTimeParam, GetWorld()->TimeSeconds);
			if (!HealthBarWidgetInstance && HealthBarWidgetClass)
			{
				HealthBarWidgetInstance = CreateWidget<UMAttachedWidget>(GetWorld(), HealthBarWidgetClass);
				//可考虑一段时间没变化就将血条隐形
				if (HealthBarWidgetInstance && HealthBarWidgetInstance->SetAttachedActorIfNot(this))
				{
					HealthBarWidgetInstance->AddToViewport();
				}
			}
		}
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


void AMAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddUniqueDynamic(this, &AMAICharacter::OnSeePawn);
	AttributeComp->OnHealthChanged.AddUniqueDynamic(this, &AMAICharacter::OnHealthChanged);
	AttributeComp->OnActorKilled.BindUObject(GetWorld()->GetAuthGameMode<AUEtestGameModeBase>(), &AUEtestGameModeBase::OnMinionKilled);
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

