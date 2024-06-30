// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DataDrivenExampleCharacter.generated.h"

class UMeshDataAsset;

UCLASS(Blueprintable)
class ADataDrivenExampleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADataDrivenExampleCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeadingConfig")
	TArray<UMeshDataAsset*> HeadingConfigArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeadingConfig")
	TObjectPtr<UMeshDataAsset> CurrentHeadingConfig=nullptr;

	UFUNCTION(BlueprintCallable,Category="HeadingConfig")
	bool ChangeConfigWithIndex(const int32 TargetIndex);

	UFUNCTION(BlueprintCallable,Category="HeadingConfig")
	bool ChangeConfigWithCVar();

	UFUNCTION(BlueprintCallable,BlueprintPure,Category="HeadingConfig")
	bool IsChangeable();

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};
