#pragma once
#include "HAL/PlatformType.h"
#include "Container/String.h"
#include "Container/Array.h"
#include "Math/Matrix.h"
#include "Math/Plane.h"
#include "Container/Map.h"
#include "fbxsdk.h"


// Explicitly qualify FbxAxisSystem to resolve ambiguity
inline fbxsdk::FbxAxisSystem unrealAxis(
    fbxsdk::FbxAxisSystem::eZAxis,      // UpVector = Z axis
    fbxsdk::FbxAxisSystem::eParityOdd,  // FrontVector = +X axis
    fbxsdk::FbxAxisSystem::eLeftHanded  // 좌표계 = Left-Handed
);

struct FBone;
struct FSkeletalMeshVertex;
struct FSkeletalMeshBoneWeight;
struct FMaterialSubset;
struct FStaticMeshVertex;
struct FStaticMeshRenderData;
struct FSkeletalMeshRenderData;
class UStaticMesh;
class USkeletalMesh;
class UMaterial;

struct FSkeletonBone
{
    FString Name;
    int32 ParentIndex;
    FMatrix LocalBindPose;
    FMatrix GlobalPose;
};

static FMatrix FbxAMatrixToFMatrix(const FbxAMatrix& InM)
{
    FMatrix Out;

    for (int Row = 0; Row < 4; ++Row)
    {
        for (int Col = 0; Col < 4; ++Col)
        {
            Out.M[Row][Col] = static_cast<float>(InM.Get(Col, Row));
        }
    }

    return Out;
}

static FbxAMatrix FMatrixToFbxAMatrix(const FMatrix& InM)
{
    FbxAMatrix Out;

    for (int Row = 0; Row < 4; ++Row)
    {
        for (int Col = 0; Col < 4; ++Col)
        {
            Out.mData[Col][Row] = static_cast<double>(InM.M[Row][Col]); // transpose
        }
    }

    return Out;
}

struct FFBXLoader
{
public:
    FFBXLoader() = default;
    ~FFBXLoader();

    static bool Initialize();
    static bool LoadFBX(const FString& FilePath);
    static void DumpAllMeshes(FbxNode* node);
    static bool FindMesh(FbxNode* Node, const FString& FilePath);

    static bool IsStaticMesh(FbxMesh* Mesh);
    static bool IsSkeletalMesh(FbxMesh* Mesh);

    static void BuildSkeletalBones(FbxMesh* Mesh, TArray<FBone>& OutBones);
    static void BuildBoneWeights(FbxMesh* Mesh, TArray<FSkeletalMeshBoneWeight>& OutWeights);
    static void BuildSkeletalVertexBuffers(FbxMesh* Mesh, TArray<FSkeletalMeshVertex>& OutVerts, TArray<uint32>& OutIndices);
    static void SetupMaterialSubsets(FbxMesh* Mesh, TArray<FMaterialSubset>& OutSubsets);
    static void LoadMaterialInfo(FbxNode* Node);
    static void UpdateSkinningMatrices(const TArray<FMatrix>& GlobalBoneTransforms, TArray<FBone>& Bones);
    //static void SkinVertexPosition(const )
public:
    static void ExtractSkeleton(FbxMesh* Mesh, TArray<FSkeletonBone>& OutBones);
    static void RecalculateGlobalPoses(TArray<FSkeletonBone>& Bones);
    static void RotateBones(TArray<FSkeletonBone>& Bones, int32 BoneIndex, const FbxVector4& EulerDegrees);
    static void ReskinVerticesCPU(FbxMesh* Mesh, const TArray<FSkeletonBone>& Bones, TArray<FSkeletalMeshVertex>& Vertices);
    static int32 FindBoneByName(const TArray<FSkeletonBone>& Bones, const FString& Name);
public:
    static void CopyControlPoints(FbxMesh* Mesh,TArray<FStaticMeshVertex>& OutVerts);
    static void BuildStaticIndexBuffer(FbxMesh* Mesh, TArray<uint32>& OutIndices);
    static void CopyNormals(FbxMesh* Mesh, TArray<FStaticMeshVertex>& OutVerts);
    static void CopyUVs(FbxMesh* Mesh, TArray<FStaticMeshVertex>& OutVerts);
    static void CopyTangents(FbxMesh* Mesh, TArray<FStaticMeshVertex>& OutVerts);
    static void ComputeBoundingBox(const TArray<FStaticMeshVertex>& InVerts, FVector& OutMin, FVector& OutMax);

private:
    inline static FbxManager* Manager = nullptr;
    inline static FbxImporter* Importer = nullptr;
    inline static FbxScene* Scene = nullptr;
    inline static FbxMesh* Mesh = nullptr;

};

struct FFBXManager
{
public:
    static UStaticMesh* CreateStaticMesh(const FString& filePath);
    static USkeletalMesh* CreateSkeletalMesh(const FString& filePath);

    inline static FStaticMeshRenderData* StaticMeshRenderData = nullptr;
    inline static FSkeletalMeshRenderData* SkeletalMeshRenderData = nullptr;
    inline static TArray<FSkeletonBone> Bones;
};

