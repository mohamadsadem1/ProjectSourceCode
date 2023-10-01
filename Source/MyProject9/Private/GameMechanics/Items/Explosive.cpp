// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/Items/Explosive.h"
#include "Characters/Player_Character/SChar.h"
#include "Components/SphereComponent.h"
#include "Characters/Player_Character/SChar.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"




AExplosive::AExplosive()
{

	Damage = 15.f;


}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);


		if (OtherActor && OtherActor != this && OtherComp && Cast<ASChar>(OtherActor))
		{
			ASChar* SChar = Cast<ASChar>(OtherActor);
			if (SChar)
			{
				SChar->DecrementHealth(Damage);
				if (OverlapParticle)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator(0.f), true);

				}
				if (OverlapSound)
				{

					UGameplayStatics::PlaySound2D(this, OverlapSound);
				}

				Destroy();

			}
		}
	

}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}
