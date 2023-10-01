// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterMechanics/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{

	
	Health = 100.f;

	Power = 150.f;

	Max_Health = 100.f;

	Max_Power = 150.f;

}




bool UAttributeComponent::ApplyHeatlthChange(float Dealta)
{
	Health += Dealta;
	
	return true;
}





