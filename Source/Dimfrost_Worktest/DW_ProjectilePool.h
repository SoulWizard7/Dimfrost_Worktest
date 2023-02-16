
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_ProjectilePool.generated.h"

class ADimfrost_WorktestProjectile;

UCLASS()
class ADW_ProjectilePool : public AActor
{
	GENERATED_BODY()
public:	
	ADW_ProjectilePool();
	
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<ADimfrost_WorktestProjectile> ProjectileClass;

	UFUNCTION()
	ADimfrost_WorktestProjectile* GetProjectile();

	UFUNCTION()
	void ReturnProjectile(ADimfrost_WorktestProjectile* proj);
	
private:
	UPROPERTY()
	TArray<ADimfrost_WorktestProjectile*> ProjPool;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
