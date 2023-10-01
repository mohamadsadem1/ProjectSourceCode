// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawVolume.generated.h"

UCLASS()
class MYPROJECT9_API ASpawVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawVolume();

	/*
				SPAWNING LOGIC
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning | Logic")
		int32 NumberAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning | Logic")
		int32 MaxNumberAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning | Logic")
		int32 NumberEnemySpawned;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning | Logic")
		int32 MaxNumberEnmeySpawned;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning | Logic")
		bool bOverlapingWithVolumeBox;
	int32 Index1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning | Logic")
	bool bAbleToSpawn;

	/*
				ENEMIES TO SPAWN/ARRAY
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
		class UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AActor> Actor_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AActor> Actor_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AActor> Actor_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AActor> Actor_4;

	TArray<TSubclassOf<AActor>> SpawnArray;


	/*
				SPELL TO SPAWN/ARRAY
	*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AActor> Spell_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AActor> Spell_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AActor> Spell_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AActor> Spell_4;

	TArray<TSubclassOf<AActor>>ItemArray;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
		TArray <class AEnemy*> EnemiesAliveArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
		int32 arraysize;



protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:


	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//GET THE LOCATION WHERE THE ENEMY SHOULD SPAWN
	FVector GetSpawnPoint();
	
	//GET THE ENEMY FROM TEH ARAY TO SPAWN
	TSubclassOf<AActor> GetSpawnActor();

	//GET THE SPELL FROM TEH ARRAY
	TSubclassOf<AActor> GetSpellActor();


	// FUNCTIONS TO TO CHOOSE THE ACTOR TYPER FROM TEH ARRAY AND THE LOCATION
	void SpawnOurActor(UClass* ToSpawn, const FVector& Location);

	void SpawnOurSpell(UClass* ToSpawn, const FVector& Location);

	// TO BE CALLED WHEN THE ENEMY DIE
	void OnEnemyDeath(AEnemy* Enemy);


	UFUNCTION(BlueprintCallable)
		void StartSpawning(float SpawnDelay);

	UFUNCTION(BlueprintCallable)
		void StartSpawningSpell(float SpawnDelay);

	UFUNCTION(BlueprintCallable)
		void StopSpawning();

	UFUNCTION(BlueprintCallable)
		void SpawnEnemy();

	FTimerHandle SpawnTimer;
	float TimeDealy;

	FORCEINLINE void SetNumberAlive(int32 Number) { NumberAlive = Number; }

	FORCEINLINE int32 GetNumberAlive() { return NumberAlive; };

	FORCEINLINE void SetNumberSpawned(int32 Number) { NumberEnemySpawned = Number; }

	FORCEINLINE int32 GetNumberSpawned() { return NumberEnemySpawned; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetAbleToSpawn(bool Cond) { bAbleToSpawn = Cond; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetAbleToSpawn() { return bAbleToSpawn; }



	UFUNCTION(BlueprintCallable)
	virtual void VolumeOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	virtual void VolumeOnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};