#include "StateMachineAnimInstance.h" 
#include "Animation/StateMachine.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimTypes.h"
#include "UObject/Casts.h"
#include "UObject/ObjectFactory.h"
#include "Engine/AssetManager.h"
#include "Components/Mesh/SkeletalMeshComponent.h"
#include "Contents/Actors/ARandCharacter.h"

// 원래는 Script 또는 Blueprint로 해야할 작업에 해당함
UStateMachineAnimInstance::UStateMachineAnimInstance() : StateMachine(nullptr)
{ 
    // [TEMP] Get asset
    IdleAnimation = Cast<UAnimSequence>(UAssetManager::Get().GetAnimationAsset(L"Contents/Idle_mixamo.com"));
    WalkAnimation = Cast<UAnimSequence>(UAssetManager::Get().GetAnimationAsset(L"Contents/Walk_mixamo.com"));;
    JumpAnimation = Cast<UAnimSequence>(UAssetManager::Get().GetAnimationAsset(L"Contents/Jump_mixamo.com"));;

    StateMachine = FObjectFactory::ConstructObject<UStateMachine>(this);
    StateMachine->OwningAnimInstance = this;

    // 상태 등록
    if (IdleAnimation)
    {
        int32 IdleStateIdx = StateMachine->AddState(TEXT("Idle"), IdleAnimation);
        StateMachine->SetEntryState(IdleStateIdx);
    }
    if (WalkAnimation)
    {
        StateMachine->AddState(TEXT("Walk"), WalkAnimation);
    }
    if (JumpAnimation)
    {
        StateMachine->AddState(TEXT("Jump"), JumpAnimation);
    }

    int32 IdleIdx = StateMachine->States.IndexOfByPredicate([](const FAnimationState& S) { return S.StateName == TEXT("Idle"); });
    int32 WalkIdx = StateMachine->States.IndexOfByPredicate([](const FAnimationState& S) { return S.StateName == TEXT("Walk"); });
    int32 JumpIdx = StateMachine->States.IndexOfByPredicate([](const FAnimationState& S) { return S.StateName == TEXT("Jump"); });

    // 전환 규칙 등록
    if (IdleIdx != INDEX_NONE && WalkIdx != INDEX_NONE) {
        StateMachine->AddTransition(IdleIdx, WalkIdx, &UStateMachineAnimInstance::Cond_CanWalk, TEXT("IdleToWalk"));
    }
    if (WalkIdx != INDEX_NONE && IdleIdx != INDEX_NONE) {
        StateMachine->AddTransition(WalkIdx, IdleIdx, &UStateMachineAnimInstance::Cond_CanIdle, TEXT("WalkToIdle"));
    }
    if (IdleIdx != INDEX_NONE && JumpIdx != INDEX_NONE) {
        StateMachine->AddTransition(IdleIdx, JumpIdx, &UStateMachineAnimInstance::Cond_CanJump, TEXT("IdleToJump"));
    }
    if (WalkIdx != INDEX_NONE && JumpIdx != INDEX_NONE) {
        StateMachine->AddTransition(WalkIdx, JumpIdx, &UStateMachineAnimInstance::Cond_CanJump, TEXT("WalkToJump"));
    }
    if (IdleIdx != INDEX_NONE && JumpIdx != INDEX_NONE) {
        StateMachine->AddTransition(JumpIdx, IdleIdx, &UStateMachineAnimInstance::Cond_CanIdle, TEXT("JumpToIdle"));
    }

    StateMachine->Initialize();
}

UStateMachineAnimInstance::~UStateMachineAnimInstance()
{
}

void UStateMachineAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds); // 부모의 업데이트 로직 (예: 시간 누적)

    USkeletalMeshComponent* SkelMeshComp = GetSkelMeshComponent();
    AActor* OwnerActor = SkelMeshComp ? SkelMeshComp->GetOwner() : nullptr;
    
    // Player Actor로 부터 데이터 업데이트
    if (ARandCharacter* Character = Cast<ARandCharacter>(OwnerActor)) {
        //bIsJumping = Character->GetCharacterMovement()->IsFalling();
    }

    if (StateMachine)
    {
        StateMachine->Update(DeltaSeconds);

        // 스테이트 머신으로부터 포즈 추출
        FPoseContext StateMachineOutput(this); // 결과 저장 위치
        FAnimExtractContext ExtractContext;

        StateMachine->Evaluate(StateMachineOutput, ExtractContext);

        for (int32 i = 0; i < StateMachineOutput.Pose.BonContainer.BoneLocalTransforms.Num(); i++)
        {
            OwningComponent->GetSkeletalMesh()->SetBoneLocalMatrix(i, StateMachineOutput.Pose.BonContainer.BoneLocalTransforms[i]);
        }

        OwningComponent->GetSkeletalMesh()->UpdateWorldTransforms();
        OwningComponent->GetSkeletalMesh()->UpdateAndApplySkinning();
    }
}

bool UStateMachineAnimInstance::Cond_CanWalk(const UAnimInstance* InAnimInstance)
{
    const UStateMachineAnimInstance* StateMachineInstance = Cast<UStateMachineAnimInstance>(InAnimInstance);
    return StateMachineInstance && StateMachineInstance->bIsMoving && !StateMachineInstance->bIsJumping;
}

bool UStateMachineAnimInstance::Cond_CanIdle(const UAnimInstance* InAnimInstance)
{
    const UStateMachineAnimInstance* StateMachineInstance = Cast<UStateMachineAnimInstance>(InAnimInstance);
    return StateMachineInstance && !StateMachineInstance->bIsMoving && !StateMachineInstance->bIsJumping;
}

bool UStateMachineAnimInstance::Cond_CanJump(const UAnimInstance* InAnimInstance)
{
    const UStateMachineAnimInstance* StateMachineInstance = Cast<UStateMachineAnimInstance>(InAnimInstance);
    return StateMachineInstance && StateMachineInstance->bIsJumping;
}
