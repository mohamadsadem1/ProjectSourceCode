// Fill out your copyright notice in the Description page of Project Settings.


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
#include "Interactions/SInteractComponent.h"
#include "Characters/CharacterMechanics/AttributeComponent.h"
#include "DrawDebugHelpers.h"

ASChar::ASChar()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	InteractionComp = CreateDefaultSubobject<USInteractComponent>(TEXT("InteractionComp"));

//	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComp"));

	bDead = false;
	Power = 150.f;

	Max_Power = 150.f;
	Coin=0;

	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	bShiftKeyDown = false;
	
	ScharSpeed = 10;
	bFDown = false;

	 Section = 0;

	bEquippedWeapon = false;


	MovementStatus = EMovementStatus::EMS_Normal;
	PowerStatus = EPowerStatus::EPS_Normal;

	PowerDrainRate = 25.f;
	MinSprintPower = 50.f;
	bHasWeapon = false;
	bAttacking = false;

	bInterpToEnemy = false;
	InterpSpeed = 15.f;

	bHasACombatTarget = false;

	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 225.f;

	BaseEyeHeight = 74.f;
}

// Called when the game starts or when spawned
void ASChar::BeginPlay()
{
	Super::BeginPlay();

	ScharPlayerController = Cast<AScharPlayerController>(GetController());

	SetInterpToEnmey(false);


}

// Called every frame
void ASChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(MovementStatus == EMovementStatus::EMS_DEAD)return;
	float DeltaPower = PowerDrainRate * DeltaTime;
	switch (PowerStatus)//switch the power status relatively to the power value
	{
		//make multiple cases of power status
	case EPowerStatus::EPS_Normal://if the player is in the normal mode and not sprinting
		if (bShiftKeyDown)// while pressing shift key and player was on the normal case
		{
			if (Power - DeltaPower <= MinSprintPower)// if the power is below the range (min sprintpower)
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

		if  (bShiftKeyDown)// if shift key down and the player is exhausted==power <min sprint power and power decremented to 0
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

	//INTERACTION FUNCTION

	if(GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime)>InteractionCheckFrequency)
	{
		PerformInteractionCheck();

	}


}



FRotator ASChar::GetLookAtRotationYawForEnemy(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

// Called to bind functionality to input
void ASChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//=============================================================
	// BASIC CHARACTER MOVEMENT
	//=============================================================
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASChar::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASChar::MoveRight);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASChar::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASChar::ShiftKeyUp);

	//==============================================================
	// FUNCTIONALITY 
	////=============================================================


	PlayerInputComponent->BindAction("F", IE_Pressed, this, &ASChar::FDown);
	PlayerInputComponent->BindAction("F", IE_Released, this, &ASChar::FUp);

	PlayerInputComponent->BindAction("LeftMouseBottom", IE_Pressed, this, &ASChar::LMBDown);
	PlayerInputComponent->BindAction("LeftMouseBottom", IE_Released, this, &ASChar::LMBUp);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASChar::PrimaryAttack);
//	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASChar::PrimaryInteract);

}



void ASChar::MoveRight(float Axis) {
	if (MovementStatus!=EMovementStatus::EMS_DEAD && (Controller != nullptr) && (Axis != 0.0f) && (!bAttacking)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

void ASChar::MoveForward(float Axis) {
	if (MovementStatus != EMovementStatus::EMS_DEAD && (Controller != nullptr) && (Axis != 0.0f) && (!bAttacking) ){
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}
void ASChar::ShowPickupLocations()
{
	for (int32 i = 0; i < PickupLocation.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, PickupLocation[i], 25.f, 12, FLinearColor::White, 5.f, 2.f);


	}


}

//void ASChar::PrimaryInteract()
//{
//	if (InteractionComp)
//	{
//		InteractionComp->PrimaryInteract();
//	}
//}

//==============================================================
//		MECHANICS
//==============================================================



//attack function

void ASChar::Attack()
{
	SetInterpToEnmey(true);
	bInterpToEnemy = true;
	if (!bAttacking && (MovementStatus != EMovementStatus::EMS_DEAD)) 
	{
		bAttacking = true;
	

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && ScharCombatMontage && !AnimInstance->Montage_IsPlaying(ScharCombatMontage))
		{
	/*		int32 x1 = FMath::RandRange(0, 1);

			if (x1 == 0)
			{
				Section = 0;
			}
			else
				
	*/

		//	switch (Section)
			
				if (GetSection() == 0)
				{
					AnimInstance->Montage_Play(ScharCombatMontage, 2.f);

					AnimInstance->Montage_JumpToSection(FName("attack_1"), ScharCombatMontage);

					SetSection(1);
					Section = 1;
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

float ASChar::GetCharacterSpeed()
{

	FVector Speed = GetVelocity();
	FVector LaterSpeed = FVector(Speed.X, Speed.Y, 0.f);
	ScharSpeed = LaterSpeed.Size();
	return ScharSpeed;
}

void ASChar::AttackEnd()
{
	SetInterpToEnmey(false);
	bInterpToEnemy = false;
	bAttacking = false;
	if (bLMBDown)
	{
		Attack();
	}
}




void ASChar::SetInterpToEnmey(bool Interp)
{
	bInterpToEnemy = Interp;
}

//
//F bottom
//
void ASChar::FDown()
{
	bFDown = true;
	if (ActiveOverlapingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlapingItem);



		if (Weapon && GetCharacterSpeed() < 5)
		{

			Weapon->Equip(this);
			bEquippedWeapon = true;

			SetActiveOverlapingItem(nullptr);
			bHasWeapon = true;
		}
		Weapon->OnEquip();
	}
}

	void ASChar::FUp()
{
	bFDown = false;

}

//
//left mouse bottom
//
void ASChar::LMBDown()
{
	bLMBDown = true;

	if (MovementStatus == EMovementStatus::EMS_DEAD)return;

	
		if (EquippedWeapon!=nullptr)
		{
			Attack();
			SetAttackingStatus(EAttackingStatus::EAS_IsAttacking);
			bAttacking = true;
		}

	
		
}

void ASChar::LMBUp()
{
	bLMBDown = false;
	if (EquippedWeapon && bAttacking == true)
	{
		bAttacking = false;
		SetAttackingStatus(EAttackingStatus::EAS_NotAttacking);
		bAttacking = false;

	}
}

//
//shift key
//
void ASChar::ShiftKeyDown()
{
	bShiftKeyDown = true;

}

void ASChar::ShiftKeyUp()
{
	bShiftKeyDown = false;

}

/*
		STATS
*/

float ASChar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{


	if ( Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die();

		if (DamageCauser)
		{
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
			if (Enemy)
			{
				Enemy->bHasValidTarget = false;
			}
		}
	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}

//
//coins
//
void ASChar::IncrementCoin(int32 coins)
{
	Coin+=coins;
}

//
//health
//
void ASChar::IncrementHealth(float Amount)
{
	if (Health + Amount >= Max_Health)
	{
		Health = Max_Health;
	}
	else
		Health += Amount;
}

void ASChar::DecrementHealth(float Amount)
{
	if (Health - Amount <= 0)
	{
		Health = 0;
	}
	else
		Health -= Amount;

}

//
//die
//
void ASChar::Die()
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ScharCombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	SetMovementStatus(EMovementStatus::EMS_DEAD);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASChar::Jump()
{
	if (MovementStatus != EMovementStatus::EMS_DEAD)
	{
		Super::Jump();
	}
}


/*
		ENUMS
*/


//==================================================================
//					Movement Status
//==================================================================
void ASChar::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting )
	
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	
	}
	else 
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}


}

//
//set equipped weapon
//
void ASChar::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;

}


/*
		STATS
*/
void ASChar::PlaySwordSwingSound()
{

	if (SwordSwingSound)
	{
		UGameplayStatics::PlaySound2D(this, SwordSwingSound);
	}
}

bool ASChar::Alive() 
{
	return true;
}

void ASChar::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void ASChar::UpdateCombatTarget()
{
	TArray<AActor*>OverlapingActors;

	GetOverlappingActors(OverlapingActors, EnemyFilter);

	if (OverlapingActors.Num() == 0)
	{
		if (ScharPlayerController)
		{
			ScharPlayerController->RemoveEnemyHealthBar();
		}
		return;

	}
	
	AEnemy* ClosestEnemy = Cast<AEnemy>( OverlapingActors[0]);

	if (ClosestEnemy)
	{
		FVector Location = GetActorLocation();

		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();

		for (auto Actor : OverlapingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();

				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;

					ClosestEnemy = Enemy;
				}
			}
		}
		if (ScharPlayerController)
		{
			ScharPlayerController->DisplayEnemyhealthBar();
		}
		SetCombatTarget(ClosestEnemy);

		bHasACombatTarget = true;
	}
}

void ASChar::PrimaryAttack()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("hand_rSocket");
	FTransform SpawnIM = FTransform(GetActorRotation(), HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnIM, SpawnParams);
}




//===================================================================================
// FUNCTIONS
//===================================================================================

void ASChar::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	
	FVector TraceStart = GetPawnViewLocation() ;

	FVector TraceEnd= TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)  ;

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

	if (LookDirection > 0)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 2.f, 0, 2);

		FCollisionQueryParams QueryParam; 

		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParam))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				const float Distance = (TraceStart - TraceHit.ImpactPoint).Size();

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable && Distance <= InteractionCheckDistance)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
					
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}

		}
			NoInteractableFound();
	}
}

void ASChar::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
		EndInteract();
	if(InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}
	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;
	TargetInteractable->BeginFocus();
}

void ASChar::NoInteractableFound()
{
	if(IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if(InteractionData.CurrentInteractable)
		if(IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

	// hide the interaction widget on the HUD

	InteractionData.CurrentInteractable = nullptr;
	TargetInteractable = nullptr;
}

void ASChar::BeginInteract()
{
	PerformInteractionCheck();

	if(InteractionData.CurrentInteractable)
	{
		if(IsValid( TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if(FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration,0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction, this, &ASChar::Interact
					, TargetInteractable->InteractableData.InteractionDuration, false);
			}
		}
	}
}

void ASChar::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndInteract();
		}
}

void ASChar::Interact()
{

	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact();
		
	}
}