// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintPreview.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APaintPreview::APaintPreview()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���b�V���R���|�[�l���g��ݒ�
	PreviewModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = PreviewModel;

	//���b�V����ݒ�
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Model/ST_Sphere"));
	PreviewModel->SetStaticMesh(Mesh);

	//�}�e���A����ݒ�
	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Game/Material/DefaultPreviewMaterial"));
	PreviewModel->SetMaterial(0,Material);

	//�����V�~�����[�V�������I��
	PreviewModel->SetSimulatePhysics(true);

	//PreviewModel�̃X�P�[���𒲐�
	PreviewModel->SetRelativeScale3D(FVector(0.3f,0.3f, 0.3f));

	//SpringArm�̃R���|�[�l���g��ݒ�E���[�g�ɃA�^�b�`
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);

	//SpringArm�̊p�x�E�����𒲐�
	//SpringArm->SetRelativeRotation(FRotator(-30.0f, 270.0f, 0.0f));
	SpringArm->TargetArmLength = 80.0f;

	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Input/IM_Controls"));

	SpringArm->bUsePawnControlRotation = true;

	// SpringArm����̊p�x���p�����Ȃ�
	//SpringArm->bInheritPitch = false;
	//SpringArm->bInheritYaw = false;
	//SpringArm->bInheritRoll = false;

	SpringArm->bEnableCameraLag = true;

	//�J�����R���|�[�l���g��ǉ�
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);

	Camera->bConstrainAspectRatio = true;

	Camera->PostProcessSettings.MotionBlurAmount = 0.0f;


	LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Action/IA_Look"));

}

// Called when the game starts or when spawned
void APaintPreview::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//�R���g���[���[�̏����p�x��ݒ�
	UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(FRotator(-30.f, 270.f, 0.0f));
	
}


// Called to bind functionality to input
void APaintPreview::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APaintPreview::Look);
	}

}

void APaintPreview::Look(const FInputActionValue& Value) {

	const FVector2D v = Value.Get<FVector2D>();

	if (Controller != nullptr) {

		// add yaw and pitch input to controller
		AddControllerYawInput(v.X);
		AddControllerPitchInput(v.Y);

		// Pawn�������Ă���Control�̊p�x���擾����
		FRotator controlRotate = GetControlRotation();

		//y���̊p�x����
		double LimitPitchAngle = FMath::ClampAngle(controlRotate.Pitch, -40.0f, -10.0f);

		// PlayerController�̊p�x��ݒ肷��
		UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(FRotator(LimitPitchAngle, controlRotate.Yaw, 0.0f));
	}
}

