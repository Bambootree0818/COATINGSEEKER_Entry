// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InGamePlayerController.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class COATINGSEEKER_ENTRY_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Constructor
	AInGamePlayerController();

	// APlayerController����Override����
	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;

	// Pause���j���[��\������
	void DispPause(const FInputActionValue& Value);

	private:
	/** Pause Input Action */
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PauseAction;
	
};
