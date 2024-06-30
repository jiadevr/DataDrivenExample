

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshDataAsset.generated.h"

UCLASS(BlueprintType)
class DATADRIVENEXAMPLE_API UMeshDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeadingConfig")
	TObjectPtr<UStaticMesh> HeadingMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeadingConfig")
	FVector BaseScale = FVector(0.25f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeadingConfig")
	float UniformScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeadingConfig")
	TObjectPtr<UMaterialInterface> MeshMaterial=nullptr;

	UFUNCTION(BlueprintCallable, Category="HeadingConfig")
	FVector GetScaleResult() const { return BaseScale * UniformScale; }
};
