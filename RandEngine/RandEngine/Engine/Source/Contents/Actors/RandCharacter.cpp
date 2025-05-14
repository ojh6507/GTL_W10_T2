#include "RandCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/Mesh/SkeletalMeshComponent.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SpringArmComponent.h"

RandCharacter::RandCharacter()
{
}

void RandCharacter::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();
    
    SphereComponent = AddComponent<USphereComponent>(FName("SphereComponent_0"));
    SetRootComponent(SphereComponent);

    SkeletalComponent = AddComponent<USkeletalMeshComponent>(FName("SkeletalComponent_0"));
    SkeletalComponent->SetupAttachment(SphereComponent);
    SkeletalComponent->SetSkeletalMesh(UAssetManager::Get().GetSkeletalMesh(L"Contents/RealSharkry.fbx"));

    USpringArmComponent* SpringArmComp = AddComponent<USpringArmComponent>(FName("SpringArmComponent_0"));
    SpringArmComp->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
    SpringArmComp->SetupAttachment(SphereComponent);

    UCameraComponent* CameraComp = AddComponent<UCameraComponent>(FName("CameraComponent_0"));
    CameraComp->SetupAttachment(SpringArmComp);
}

UObject* RandCharacter::Duplicate(UObject* InOuter)
{
    ThisClass* NewActor = Cast<ThisClass>(Super::Duplicate(InOuter));

    return NewActor;
}

void RandCharacter::BeginPlay()
{
    APlayer::BeginPlay();
}

void RandCharacter::Tick(float DeltaTime)
{
    APlayer::Tick(DeltaTime);
}
