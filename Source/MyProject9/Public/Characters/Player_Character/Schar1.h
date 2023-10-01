// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SChar.h"
#include "Schar1.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT9_API ASchar1 : public ASChar
{
	GENERATED_BODY()
	
protected:


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input


	void Attack() override;


};
