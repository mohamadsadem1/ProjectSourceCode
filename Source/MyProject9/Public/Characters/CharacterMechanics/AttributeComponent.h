// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT9_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | PlayerStat")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | PlayerStat")
	float Max_Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player | PlayerStat")
	float Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | PlayerStat")
	float Max_Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | PlayerStat")
	int32 Coin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Running")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Running")
	float SprintingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float PowerDrainRate;

	UPROPERTY(EditAnywhere)
	float Power_treshold;

public:	

	UFUNCTION(BlueprintCallable,Category="Attributes")
	bool ApplyHeatlthChange(float Dealta);




		
};
