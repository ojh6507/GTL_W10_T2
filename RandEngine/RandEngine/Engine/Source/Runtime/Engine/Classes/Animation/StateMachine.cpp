#include "StateMachine.h"
#include "Animation/AnimTypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimSequence.h"
#include "Userinterface/Console.h"

UStateMachine::UStateMachine()
    : EntryStateIndex(INDEX_NONE),
    CurrentStateIndex(INDEX_NONE),
    TimeInCurrentState(0.0f),
    OwningAnimInstance(nullptr)
{
}

UStateMachine::UStateMachine(UAnimInstance* InOwningAnimInstance)
    : EntryStateIndex(INDEX_NONE),
    CurrentStateIndex(INDEX_NONE),
    TimeInCurrentState(0.0f),
    OwningAnimInstance(InOwningAnimInstance)
{
}

UStateMachine::~UStateMachine()
{
}

int32 UStateMachine::AddState(FName StateName, UAnimSequence* InAnimaSequence)
{
    States.Emplace(StateName, InAnimaSequence);
    return States.Num() - 1;
}

void UStateMachine::AddTransition(int32 FromStateIdx, int32 ToStateIdx, FAnimationTransitionRule::TransitionConditionFunc Condition, FName RuleName)
{
    if (States.IsValidIndex(FromStateIdx) && States.IsValidIndex(ToStateIdx))
    {
        States[FromStateIdx].AddTransitionRule(ToStateIdx, Condition, RuleName);
    }
    else
    {
         UE_LOG(ELogLevel::Display, TEXT("UStateMachine::AddTransition: Invalid state index. From: %d, To: %d"), FromStateIdx, ToStateIdx);
    }
}

void UStateMachine::SetEntryState(int32 StateIdx)
{
    if (States.IsValidIndex(StateIdx))
    {
        EntryStateIndex = StateIdx;
    }
}

void UStateMachine::Initialize()
{
    if (States.IsValidIndex(EntryStateIndex))
    {
        CurrentStateIndex = EntryStateIndex;
        TimeInCurrentState = 0.0f;
    }
    else if (States.Num() > 0)
    {
        CurrentStateIndex = 0;
        TimeInCurrentState = 0.0f;
    }
}

void UStateMachine::Update(float DeltaTime)
{
    if (!OwningAnimInstance || !States.IsValidIndex(CurrentStateIndex))
    {
        return;
    }

    TimeInCurrentState += DeltaTime;

    const FAnimationState& ActiveState = States[CurrentStateIndex];

    for (const FAnimationTransitionRule& Rule : ActiveState.Transitions)
    {
        if (Rule.CanEnter(OwningAnimInstance))
        {
             if (States.IsValidIndex(Rule.TransitionIndex)) // 목표 상태 유효성 검사
             {
                 UE_LOG(ELogLevel::Display, TEXT("State Transition: From '%s' To '%s' via Rule '%s'"),
                      *ActiveState.StateName.ToString(),
                      *States[Rule.TransitionIndex].StateName.ToString(),
                      *Rule.RuleToExecute.ToString());
             }

            CurrentStateIndex = Rule.TransitionIndex;
            TimeInCurrentState = 0.0f;
            break; // 첫 번째 유효한 전환을 바로 적용
        }
    }
}

void UStateMachine::Evaluate(FPoseContext& OutPoseContext, FAnimExtractContext& InOutExtractContext)
{
    if (States.IsValidIndex(CurrentStateIndex))
    {
        const FAnimationState& ActiveState = States[CurrentStateIndex];
        
        const float PlayLength = static_cast<float>(ActiveState.AnimationToPlay->GetDataModel()->GetPlayLength());
        float SampleTime = FMath::Fmod(TimeInCurrentState, PlayLength);

        // FAnimationState::Evaluate(float CurrentStateTime, FPoseContext& OutPoseContext, FAnimExtractContext& InOutExtractContext)
        ActiveState.Evaluate(SampleTime, OutPoseContext, InOutExtractContext);
    }
    else
    {
        // 유효한 상태가 없는 케이스
    }
}

FName UStateMachine::GetCurrentStateName() const
{
    if (States.IsValidIndex(CurrentStateIndex))
    {
        return States[CurrentStateIndex].StateName;
    }
    return NAME_None;
}
