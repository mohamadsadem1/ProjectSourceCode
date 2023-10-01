// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/Magic_Abilities/SMagicProjectil.h"
#include "Components/SphereComponent.h"
#include"GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Characters/CharacterMechanics/AttributeComponent.h"

// Sets default values
ASMagicProjectil::ASMagicProjectil()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	 RootComponent= SphereComp  ;
	 SphereComp->SetCollisionProfileName("Projectile");
	 SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectil::OnActorOverlap);

	 EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	 EffectComp->SetupAttachment(RootComponent);
	 
	 MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	 MovementComp->InitialSpeed = 1000.f;
	 MovementComp->bRotationFollowsVelocity = true;
	 MovementComp->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void ASMagicProjectil::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASMagicProjectil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASMagicProjectil::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
	UAttributeComponent*AttributeComp=Cast<UAttributeComponent>(OtherActor->GetComponentByClass(UAttributeComponent::StaticClass()));

	if (AttributeComp)
	{
		AttributeComp->ApplyHeatlthChange(-20.f);
		Destroy();
	}
	}
}



