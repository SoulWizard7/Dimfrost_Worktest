
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_WeaponComponent.generated.h"

class ADW_ProjectilePool;
class ADimfrost_WorktestProjectile;
class ADimfrost_WorktestCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIMFROST_WORKTEST_API UTP_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<ADW_ProjectilePool> ProjectilePoolClass;

	UPROPERTY()
	ADW_ProjectilePool* ProjectilePool;

	UPROPERTY(EditAnywhere, Category="Weapons Stats")
	float ProjectileMaxDistance;

	UPROPERTY(EditAnywhere, Category="Weapons Stats")
	float ProjectileTimeToKill = 3.f;

	UPROPERTY(EditAnywhere, Category="Weapons Stats")
	float ProjectileLaunchSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category="Weapons Stats")
	float ProjectileMaxFireAngle = 60.f;
	
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<ADimfrost_WorktestProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(ADimfrost_WorktestCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		

private:
	/** The Character holding this weapon*/
	ADimfrost_WorktestCharacter* Character;
};
