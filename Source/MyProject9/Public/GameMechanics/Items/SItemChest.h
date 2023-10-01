// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactions/SGamePlayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class MYPROJECT9_API ASItemChest : public AActor, public ISGamePlayInterface
{
	GENERATED_BODY()

public:

	void Interact_Implementation(APawn* InstigatorPawn);

		UPROPERTY(EditAnywhere)
		float TargetPitch;
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent*LidMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent * BaseMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
