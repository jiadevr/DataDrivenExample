// Copyright Epic Games, Inc. All Rights Reserved.

#include "DataDrivenExampleCharacter.h"

#include "DataDrivenWorldSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DataDrivenWorldSettings.h"
#include "Kismet/KismetStringLibrary.h"

static TAutoConsoleVariable<int32> CVarConfigIndex(
	TEXT("DA.SetCurrentConfigIndex"), 0,TEXT("Set Current Heading Config To Item In Array Of Given Index"),
	ECVF_Default);

ADataDrivenExampleCharacter::ADataDrivenExampleCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ADataDrivenExampleCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADataDrivenExampleCharacter::BeginPlay()
{
	bool bUseConfigIndex = false;
	bool bGetSuccessfully = GConfig->GetBool(TEXT("DataDriven"),TEXT("bUseConfigIndex"), bUseConfigIndex, GGameIni);
	int32 PresetIndexInConfig = 0;
	bGetSuccessfully &= GConfig->GetInt(TEXT("DataDriven"),TEXT("PresetIndex"),PresetIndexInConfig,GGameIni);
	if (bUseConfigIndex&&bGetSuccessfully&&ChangeConfigWithIndex(PresetIndexInConfig))
	{
		UE_LOG(LogTemp,Display,TEXT("Change Preset To Index %d,Driven By Ini"),PresetIndexInConfig);
	}
	else
	{
		ADataDrivenWorldSettings* DataDrivenWorldSettings = Cast<ADataDrivenWorldSettings>(
			UGameplayStatics::GetGameMode(this)->GetWorld()->GetWorldSettings());
		if (DataDrivenWorldSettings)
		{
			UMeshDataAsset* WorldHeadingConfig = DataDrivenWorldSettings->WorldDefaultHeadingConfig;
			if (WorldHeadingConfig)
			{
				CurrentHeadingConfig = WorldHeadingConfig;
				UE_LOG(LogTemp, Warning, TEXT("CurrentHeadingConfig Was Set With World Settings Data"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("Read ADataDrivenWorldSettings Config Failed,Please Check Project World Settings! "))
			if (nullptr == CurrentHeadingConfig && HeadingConfigArray.IsValidIndex(0))
			{
				CurrentHeadingConfig = HeadingConfigArray[0];
				UE_LOG(LogTemp, Warning, TEXT("CurrentHeadingConfig Was Set With HeadingConfigArray Data"));
			}
		}
	}
	Super::BeginPlay();
}

bool ADataDrivenExampleCharacter::ChangeConfigWithIndex(const int32 TargetIndex)
{
	if (!HeadingConfigArray.IsValidIndex(TargetIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Index %d"), TargetIndex);
		return false;
	}
	CurrentHeadingConfig = HeadingConfigArray[TargetIndex];
	return true;
}

bool ADataDrivenExampleCharacter::ChangeConfigWithCVar()
{
	int32 CVarIndex = CVarConfigIndex.GetValueOnGameThread();
	return ChangeConfigWithIndex(CVarIndex);
}

bool ADataDrivenExampleCharacter::IsChangeable()
{
	bool bIsChangeable = true;
	bool bGetSuccessfully = GConfig->GetBool(TEXT("DataDriven"),TEXT("bIsChangeable"), bIsChangeable, GGameIni);
	return bIsChangeable;
}
