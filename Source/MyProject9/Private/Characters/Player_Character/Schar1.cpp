// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player_Character/Schar1.h"
#include "Characters/Player_Character/SChar.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/Enemies/Enemy.h"
#include "Sound/SoundCue.h"
#include "ScharPlayerController.h"



void ASchar1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementStatus == EMovementStatus::EMS_DEAD)return;
	float DeltaPower = PowerDrainRate * DeltaTime;
	switch (PowerStatus)//switch the power status relativly to the power value
	{
		//make multiple cases of power status
	case EPowerStatus::EPS_Normal://if the player is in the normal mode and not sprinting
		if (bShiftKeyDown)// while pressing shift key and player was on the normal case
		{
			if (Power - DeltaPower <= MinSprintPower)// if the power is below the range (minsprintpower)
			{
				SetPowerStatus(EPowerStatus::EPS_BelowMinimum);//make the status of the power below the min
				Power -= DeltaPower;//continue the decrementation
			}
			else // if the power is more than the range (minsprintpower)
			{
				Power -= DeltaPower;//continue the decrementation

			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);//change the movement status from normal to sprinting (because the shift key is down)
		}

		else //if shift key up
		{
			if (Power + DeltaPower >= Max_Power)//if the power is more than the max power 
			{
				Power = Max_Power;//set the power = to the max power to make the power limited
			}
			else//if the power still below the range (max power)
			{
				Power += DeltaPower;//continue the inrementation
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);//make the movement status to normal after the shift key is not used anymore
		}

		break;

	case EPowerStatus::EPS_BelowMinimum://in case of the power is less the minsprintpower
		if (bShiftKeyDown)//if key down and the power rate is less  than the minimum sprint power
		{
			if (Power - DeltaPower <= 0.f) // if the power is <=0
			{
				SetPowerStatus(EPowerStatus::EPS_Exhausted);// set the power status as exhausted<= minsprintpower
				Power = 0.f;//set the power=0 to make the a limitation to the power
				SetMovementStatus(EMovementStatus::EMS_Normal);//make the movement status normal cause out of power
			}
			else // if the power still more than 0
			{
				Power -= DeltaPower;//still dercrement the power
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}

		}
		else  //if shift key up
		{
			if (Power + DeltaPower >= MinSprintPower)//while shift key up and the power is more than the minsprintpower 
			{
				SetPowerStatus(EPowerStatus::EPS_Normal);//switch the power status to normal 
				Power += DeltaPower;// continue the increamentation of the power 
			}
			else
			{
				Power += DeltaPower;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);//make the movement status to normal after the shift key is not used anymore

		}

		break;
	case EPowerStatus::EPS_Exhausted:

		if (bShiftKeyDown)// if shift key down and the player is exhausted==power <min sprint power and power decremented to 0
		{
			Power = 0;//limit the power by 0 
			bShiftKeyDown = false;
		}
		else//if shift key up

		{
			SetPowerStatus(EPowerStatus::EPS_ExhaustedRecovering);// stop the decreamantation and start the recovery of the power
			Power += DeltaPower;//increment the power

		}
		SetMovementStatus(EMovementStatus::EMS_Normal);//while recovering the charcter movement status should be in the normal mode

		break;
	case EPowerStatus::EPS_ExhaustedRecovering://the player here in the case of recovering the power 

		if (Power + DeltaPower >= MinSprintPower)//while the power is more than the minsprintpower
		{
			SetPowerStatus(EPowerStatus::EPS_Normal);//make the power status normal
			Power += DeltaPower;
		}
		else
		{
			Power += DeltaPower;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}

	if (bInterpToEnemy && CombatTartg)
	{

		FRotator LookAtYawn = GetLookAtRotationYawForEnemy(CombatTartg->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYawn, DeltaTime, InterpSpeed);
		SetActorRotation(InterpRotation);
	}

	if (CombatTartg)
	{
		CombatTargetLocation = CombatTartg->GetActorLocation();
		if (ScharPlayerController)
		{
			ScharPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}



void ASchar1::Attack()
{
	SetInterpToEnmey(true);
	bInterpToEnemy = true;
	if (!bAttacking && (MovementStatus != EMovementStatus::EMS_DEAD))
	{
		bAttacking = true;


		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && ScharCombatMontage && !AnimInstance->Montage_IsPlaying(ScharCombatMontage))
		{
			

			if (GetSection() == 0)
			{
				AnimInstance->Montage_Play(ScharCombatMontage, 2.f);

				AnimInstance->Montage_JumpToSection(FName("attack_1"), ScharCombatMontage);

				SetSection(1);
			}

			else if (GetSection() == 1)
			{
				AnimInstance->Montage_Play(ScharCombatMontage, 1.f);

				AnimInstance->Montage_JumpToSection(FName("attack_2"), ScharCombatMontage);

				SetSection(0);
			}




		}

	}
}
