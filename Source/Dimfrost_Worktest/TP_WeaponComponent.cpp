
#include "TP_WeaponComponent.h"
#include "Dimfrost_WorktestCharacter.h"
#include "Dimfrost_WorktestProjectile.h"
#include "DW_ProjectilePool.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	ProjectilePool = Cast<ADW_ProjectilePool>(UGameplayStatics::GetActorOfClass(GetWorld(), ADW_ProjectilePool::StaticClass()));
	if(ProjectilePool == nullptr)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ProjectilePool = GetWorld()->SpawnActor<ADW_ProjectilePool>(ProjectilePoolClass, FVector::Zero(), FRotator::ZeroRotator, ActorSpawnParams);
	}
}

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::Fire()
{
	if(Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);	

			if(FMath::Abs(SpawnRotation.Pitch) > ProjectileMaxFireAngle)
			{				
				SpawnRotation.Pitch = SpawnRotation.Pitch > 0 ? ProjectileMaxFireAngle : -ProjectileMaxFireAngle;
			}

			ADimfrost_WorktestProjectile* proj = ProjectilePool->GetProjectile();
			
			if(proj == nullptr) return;						
			
			proj->SpawnPos = SpawnLocation;
			proj->SetActorLocation(SpawnLocation);
			proj->SetActorRotation(SpawnRotation);
			proj->MaxDistance = ProjectileMaxDistance;
			proj->EnableProjectile(ProjectileTimeToKill);			
			proj->GetProjectileMovement()->Velocity = proj->GetActorForwardVector() * ProjectileLaunchSpeed;
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(Character != nullptr)
	{
		// Unregister from the OnUseItem Event
		Character->OnUseItem.RemoveDynamic(this, &UTP_WeaponComponent::Fire);
	}
}

void UTP_WeaponComponent::AttachWeapon(ADimfrost_WorktestCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if(Character != nullptr)
	{
		// Attach the weapon to the First Person Character
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetOwner()->AttachToComponent(Character->GetMesh1P(),AttachmentRules, FName(TEXT("GripPoint")));

		// Register so that Fire is called every time the character tries to use the item being held
		Character->OnUseItem.AddDynamic(this, &UTP_WeaponComponent::Fire);
	}
}

