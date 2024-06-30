// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "DataDrivenWorldSettings.generated.h"

class UMeshDataAsset;

UCLASS()
class DATADRIVENEXAMPLE_API ADataDrivenWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="HeadingConfig")
	TObjectPtr<UMeshDataAsset> WorldDefaultHeadingConfig=nullptr;
};
