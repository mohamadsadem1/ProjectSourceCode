// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGamePlayInterface.h"
#include "SInteractComponent.generated.h"

class ISGamePlayInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT9_API USInteractComponent : public UActorComponent , public ISGamePlayInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInteractComponent();

	void PrimaryInteract();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
