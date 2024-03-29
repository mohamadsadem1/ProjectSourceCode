// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include <GameMechanics/Items/Weapon.h>

#include "Interfaces/InteractionInterface.h"
#include "SChar.generated.h"

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

		FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.f)
	{

	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;


};


UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Stop	  UMETA(Display_Name = "stopped"),

	EMS_Normal    UMETA(DisplayName = "Normal"),

	EMS_Sprinting UMETA(DisplayName = "Sprinting"),

	EMS_DEAD	  UMETA(DisplayName = "Dead"),

	EMS_MAX		  UMETA(DisplayName = "DeafultMAX")
};

UENUM(BlueprintType)
enum class EPowerStatus : uint8
{
	EPS_Normal				  UMETA(DisplayName = "Normal"),
	EPS_BelowMinimum		  UMETA(DisplayName = "BelowMinimum"),
	EPS_Exhausted			  UMETA(DisplayName = "Exhausted"),
	EPS_ExhaustedRecovering   UMETA(DisplayName = "ExhaustedRecovering"),

	EPS_Max                   UMETA(DisplayName = "DefaultMax")
};

UENUM (BlueprintType)
enum class EAttackingStatus : uint8
{
	EAS_NotAttacking	UMETA(DisplayName = "IDLE"),
	EAS_IsAttacking		UMETA(DisplayName = "IsAttacking"),

	EAS_Max				UMETA(DisplayName = "Max")
};

UCLASS()
class MYPROJECT9_API ASChar: public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ASChar();
	TArray<FVector>PickupLocation;

	//=========================================================================
	//		STATS
	//=========================================================================

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Anim")
	int32 Section;

	FORCEINLINE int32 GetSection() { return Section; }

	FORCEINLINE void SetSection(int32 section) { Section = section; }

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

	void DecrementHealth(float Amount);

	UFUNCTION(BlueprintCallable)
	void IncrementCoin(int32 coins);

	UFUNCTION(BlueprintCallable)
	void IncrementHealth(float Amount);

	//=========================================================================
	//		MESHES
	//=========================================================================



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class	USInteractComponent* InteractionComp;

	//UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	//class	UAttributeComponent* AttributeComp;


	//=========================================================================
	//		MECHANICS
	//=========================================================================


	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category ="Enums")
	EMovementStatus MovementStatus;

	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bInterpToEnemy;

	void SetInterpToEnmey(bool Interp);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Combat")
	class AEnemy* CombatTartg;

	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTartg = Target; }

	FRotator GetLookAtRotationYawForEnemy(FVector Target);

	float ScharSpeed;

	bool bShiftKeyDown;

	bool bDead;

	bool bFDown;

	bool bLMBDown;

	void FDown();
	void FUp();

	void LMBDown();

	void LMBUp();

	void MoveRight(float Axis);

	void MoveForward(float Axis);

	virtual float GetCharacterSpeed();

	//press down to enable sprinting
	void ShiftKeyDown();

	//released to stop sprinting
	void ShiftKeyUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bHasWeapon;

	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	static TMap<FString, FString> EnemiesKilledByPlayer; // Static map to store enemies killed by any character


	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,AController* EventInstigator, AActor* DamageCauser)override;

	void Die();

	virtual void Jump() override;

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	bool Alive();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bHasACombatTarget;

	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasACombatTarget = HasTarget; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AScharPlayerController* ScharPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector CombatTargetLocation;

	void UpdateCombatTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat");
	TSubclassOf<AEnemy>EnemyFilter;

	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = "Attacking")
	bool bAttacking;


	//=========================================================================
	//			SOUNDS AND PARTICLES AND ANIMATION
	//=========================================================================

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UParticleSystem* HitParticle_Schar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USoundCue* HitSound_Schar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USoundCue* SwordSwingSound;

	UFUNCTION(BlueprintCallable)
	void PlaySwordSwingSound();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Anims")
	class UAnimMontage* ScharCombatMontage;




	//=========================================================================
	//		ENUMS
	//=========================================================================

	//set movement statut amd runing speed /
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EPowerStatus PowerStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "collect")
	class AWeapon * EquippedWeapon ;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "collect")
	class Acollect* ActiveOverlapingItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "collect")
	EAttackingStatus AttackingStatus;
	bool bEquippedWeapon;

	FORCEINLINE 
	void SetPowerStatus(EPowerStatus Status)
	{ 
		PowerStatus = Status; 
	}

	FORCEINLINE 
	void SetAttackingStatus(EAttackingStatus status) 
	{ 
		AttackingStatus = status;
	}

	FORCEINLINE
	EAttackingStatus GetAttackingStatus() 
	{ 
		return AttackingStatus; 
	}


	 void SetEquippedWeapon(AWeapon* WeaponToSet);

	 FORCEINLINE  AWeapon* GetEquippedWeapon()
	 {
		 return EquippedWeapon;
	 }
	
	FORCEINLINE void SetActiveOverlapingItem(Acollect* Collect) 
	{ 
		ActiveOverlapingItem = Collect;
	}
//===================================================================================
// FUNCTIONS
//===================================================================================

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor>ProjectileClass;

	UPROPERTY(VisibleAnywhere,Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface>TargetInteractable;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	void PrimaryAttack();

//	void PrimaryInteract();

};
