#include "ARandCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/Mesh/SkeletalMeshComponent.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SpringArmComponent.h"

ARandCharacter::ARandCharacter()
{
}

void ARandCharacter::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();
    SphereComponent = AddComponent<USphereComponent>(FName("SphereComponent_0"));
    SetRootComponent(SphereComponent);
    
    SkeletalComponent = AddComponent<USkeletalMeshComponent>(FName("SkeletalComponent_0"));
    SkeletalComponent->SetSkeletalMesh(UAssetManager::Get().GetSkeletalMesh(L"Contents/RealSharkry.fbx"));
    SkeletalComponent->SetupAttachment(SphereComponent);
    SkeletalComponent->SetRelativeRotation(FRotator(0,-90,0));

    USpringArmComponent* SpringArmComp = AddComponent<USpringArmComponent>(FName("SpringArmComponent_0"));
    SpringArmComp->SetupAttachment(SphereComponent);

    UCameraComponent* CameraComp = AddComponent<UCameraComponent>(FName("CameraComponent_0"));
    CameraComp->SetupAttachment(SpringArmComp);
}

UObject* ARandCharacter::Duplicate(UObject* InOuter)
{
    ThisClass* NewActor = Cast<ThisClass>(Super::Duplicate(InOuter));

    return NewActor;
}

void ARandCharacter::BeginPlay()
{
    APlayer::BeginPlay();
}

void ARandCharacter::Tick(float DeltaTime)
{
    APlayer::Tick(DeltaTime);
}
