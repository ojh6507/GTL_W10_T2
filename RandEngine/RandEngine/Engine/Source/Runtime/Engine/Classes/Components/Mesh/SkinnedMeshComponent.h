#pragma once
#include "MeshComponent.h"
#include "SkeletalMeshRenderData.h"
#include "UObject/Casts.h"

class UMaterial;
class USkinnedMeshComponent : public UMeshComponent
{
    DECLARE_CLASS(USkinnedMeshComponent, UMeshComponent)

public:
    USkinnedMeshComponent() = default;

    virtual UObject* Duplicate(UObject* InOuter) override;

    virtual uint32 GetNumMaterials() const override
    {
        return SkeletalMesh ? SkeletalMesh->GetMaterials().Num() : 0;
    }

    virtual UMaterial* GetMaterial(uint32 ElementIndex) const override;
    virtual uint32 GetMaterialIndex(FName MaterialSlotName) const override;
    virtual TArray<FName> GetMaterialSlotNames() const override;
    virtual void GetUsedMaterials(TArray<UMaterial*>& Out) const override;

    USkeletalMesh* GetSkeletalMesh() const { return SkeletalMesh; }
    void SetSkeletalMesh(USkeletalMesh* InMesh)
    {
        SkeletalMesh = InMesh;
        OverrideMaterials.SetNum(SkeletalMesh ? SkeletalMesh->GetMaterials().Num() : 0);
    }

protected:
    USkeletalMesh* SkeletalMesh = nullptr;
};
