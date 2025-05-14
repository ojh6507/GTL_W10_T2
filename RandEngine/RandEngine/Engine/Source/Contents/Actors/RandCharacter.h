#pragma once
#include "Actors/Player.h"
#include "Components/ProjectileMovementComponent.h"

class USphereComponent;

class RandCharacter : public APlayer
{
    DECLARE_CLASS(RandCharacter, APlayer)
public:
    RandCharacter();
    virtual ~RandCharacter() override = default;

    virtual void PostSpawnInitialize() override;

    UObject* Duplicate(UObject* InOuter) override;

    void BeginPlay() override;

    void Tick(float DeltaTime) override;

    UPROPERTY
    (USphereComponent*, SphereComponent, = nullptr)

    UPROPERTY
    (USkeletalMeshComponent*, SkeletalComponent, = nullptr)
};
