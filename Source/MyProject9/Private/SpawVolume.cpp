// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Characters/Enemies/Enemy.h"
#include "AIController.h"
#include "GameMechanics/Items/collect.h"
#include "TimerManager.h"
#include <Components/TimelineComponent.h>


// Sets default values
ASpawVolume::ASpawVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	 SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));

  	 bOverlapingWithVolumeBox = false;

	 NumberAlive =  0;

	 NumberEnemySpawned = 0;

	 MaxNumberAlive = 5;

	 MaxNumberEnmeySpawned = 10;

	 Index1 = 0;

	 TimeDealy = 3.f;


}

// Called when the game starts or when spawned
void ASpawVolume::BeginPlay()
{
	Super::BeginPlay();

	if (Actor_1 && Actor_2 && Actor_3 && Actor_4)
	{
		SpawnArray.Add(Actor_1);
		SpawnArray.Add(Actor_2);
		SpawnArray.Add(Actor_3);
		SpawnArray.Add(Actor_4);
	}

	SpawningBox->OnComponentBeginOverlap.AddDynamic(this, &ASpawVolume::VolumeOnOverlapBegin);

	SpawningBox->OnComponentEndOverlap.AddDynamic(this, &ASpawVolume::VolumeOnOverlapEnd);


}

// Called every frame
void ASpawVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/*
		GET THE LOCATION ON THE VOLUME BOX TO SPAWN THE ENEMY
*/
FVector ASpawVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();

	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	return Point;
}




/*
			SET TIMER AND DELAY BETWEEN EVERY ENEMY SPAWNING
*/
void ASpawVolume::SpawnEnemy()
{
	if (NumberAlive < 5 && NumberEnemySpawned < 10)
	{
		bAbleToSpawn = true;

		SpawnOurActor(GetSpawnActor(), GetSpawnPoint());

	}
	if (NumberAlive > 5 && NumberEnemySpawned > 10)

		bAbleToSpawn = false;
	return;
}

void ASpawVolume::StartSpawning(float SpawnDelay)
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawVolume::SpawnEnemy, SpawnDelay, true);
}

void ASpawVolume::StartSpawningSpell(float SpawnDelay)
{
}

void ASpawVolume::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}


/*
		FUNCTION TO SPAWN THE ENEMY
*/

void ASpawVolume::SpawnOurActor(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn)
	{
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;

		if (World)
		{
				AActor* Actor = World->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.f), SpawnParams);

				AEnemy* Enemy = Cast<AEnemy>(Actor);
				if (Enemy)
				{
					Enemy->SpawnDefaultController();

					AAIController* AICont = Cast<AAIController>(Enemy->GetController());
					if (AICont)
					{
						Enemy->AIController = AICont;

					}
					EnemiesAliveArray.Add(Enemy);

					Enemy->IsSpawnedByVolume = true;
					Enemy->SetSpawnVolume(this);

					NumberAlive++;
					SetNumberAlive(NumberAlive);
					GetNumberAlive();

					NumberEnemySpawned++;
					SetNumberSpawned(NumberEnemySpawned);
					GetNumberSpawned();
				
				}
		}
	}
}

/*
		FUNCTION TO SPAWN THE SPELL
*/

void ASpawVolume::SpawnOurSpell(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn)
	{
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;

		if (World)
		{
			AActor* Actor = World->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.f), SpawnParams);

		}
	}
}



/*
			GET WHAT ENEMY TYPE TO SPAWN && IT IS SELECTED USING BP
*/
TSubclassOf<AActor> ASpawVolume::GetSpawnActor()
{
	if (SpawnArray.Num() > 0)
	{
		int32 Selection = FMath::RandRange(0, SpawnArray.Num() - 1);

		return SpawnArray[Selection];
	}
	else
	{
		return nullptr;
	}
}

TSubclassOf<AActor> ASpawVolume::GetSpellActor()
{
	if (ItemArray.Num() > 0)
	{
		int32 Selection = FMath::RandRange(0, ItemArray.Num() - 1);

		return ItemArray[1];
		UE_LOG(LogTemp, Warning, TEXT("spawn spell"));
	}
	else
	{
		return nullptr;
	}
}





/*
			WHEN THE ENEMY DIE THIS FUNCTION WILL BE CALLED
*/
void  ASpawVolume::OnEnemyDeath(AEnemy* Enemy)
{
	if (Enemy)
	{
			EnemiesAliveArray.Remove(Enemy);

			NumberAlive--;

			SetNumberAlive(NumberAlive);

			
			FVector EnemyLocation = Enemy->GetActorLocation();

			// Spawn the spell at the location of the dead enemy
			SpawnOurSpell(Spell_1, EnemyLocation);
		
	}
}






/*
			CHARACTER OVERLAPING WITH THE SPAWNING VOLUME
*/
void ASpawVolume::VolumeOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		bOverlapingWithVolumeBox = true;


		StartSpawning(1);
	}
}
	



void ASpawVolume::VolumeOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{ 
	bOverlapingWithVolumeBox = false;


}





