// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/Items/Pickup.h"
#include "Characters/Player_Character/SChar.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"




APickup::APickup()
{


}

void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);


		if (OtherActor)
		{
			ASChar* SChar = Cast<ASChar>(OtherActor);
			if (SChar)
			{
				OnPickup(SChar);
				SChar->PickupLocation.Add(GetActorLocation());

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




void APickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
	{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	}


