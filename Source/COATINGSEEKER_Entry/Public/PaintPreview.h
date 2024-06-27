// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PaintPreview.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class COATINGSEEKER_ENTRY_API APaintPreview : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APaintPreview();

	//���f��
	UPROPERTY(EditAnywhere, Category = "Previewer")
	TObjectPtr<UStaticMeshComponent> PreviewModel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	//sprintarm�R���|�[�l���g
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	//�J�����R���|�[�l���g
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	//�}�b�s���O�R���e�L�X�g
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	//���b�N�C���v�b�g�A�N�V����
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* LookAction;

	//�Y�[���C���v�b�g�A�N�V����
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* ZoomAction;

};
