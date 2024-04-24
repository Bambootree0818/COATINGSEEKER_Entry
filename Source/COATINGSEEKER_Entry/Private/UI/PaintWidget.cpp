// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PaintWidget.h"
#include "Misc/FileHelper.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
#include "UI/CustomMaterialButton.h"
#include "Kismet/GameplayStatics.h"
#include "PaintPreview.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UI/PaintMenuHUD.h"

class UVerticalBox;

void UPaintWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ButtonSphere��OnClicked�ɁuOnButtonSphereClicked�v���֘A�Â���
	Sphere->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonSphereClicked);

	// ButtonBox��OnClicked�ɁuOnButtonBoxClicked�v���֘A�Â���
	Box->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonBoxClicked);

	// ButtonPlane��OnClicked�ɁuOnButtonPlaneClicked�v���֘A�Â���
	Plane->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonPlaneClicked);

	// ButtonMS06R��OnClicked�ɁuOnButtonMS06RClicked�v���֘A�Â���
	MS06R->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonMS06RClicked);

	Parts1->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonParts1Clicked);
	Parts2->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonParts2Clicked);
	Parts3->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonParts3Clicked);
	Parts4->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonParts4Clicked);
	Parts5->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonParts5Clicked);
	Parts6->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonParts6Clicked);

	HiddenButtons.Add(Parts1);
	HiddenButtons.Add(Parts2);
	HiddenButtons.Add(Parts3);
	HiddenButtons.Add(Parts4);
	HiddenButtons.Add(Parts5);
	HiddenButtons.Add(Parts6);

}


void UPaintWidget::LoadMaterialData() {

	//Content�f�B���N�g���̃p�X���擾
	FString BasePath = FPaths::ProjectContentDir();

	FString MaterialDataPath = BasePath + TEXT("MaterialDatas/");

	//UKismetSystemLibrary::PrintString(this, MaterialDataPath, true, true, FColor::Cyan, 10.f, TEXT("None"));

	//�t�@�C���}�l�[�W���[�̃C���X�^���X���擾
	IFileManager& FileManager = IFileManager::Get();

	//�ǂݍ��ރt�@�C�����̔z��
	TArray<FString> FileNames;

	//�w��p�X����JSON�t�@�C���̃��X�g���擾
	FileManager.FindFiles(FileNames, *MaterialDataPath, *FString("*.json"));

	// �t�@�C�����ёւ�
	auto ExtractNumber = [](const FString& FileName, bool& bHasNumber) {
		FString NumberPart;
		bHasNumber = false;
		for (const TCHAR& Character : FileName) {
			if (FChar::IsDigit(Character)) {
				NumberPart.AppendChar(Character);
				bHasNumber = true;
			}
			else if (!NumberPart.IsEmpty()) {
				break;
			}
		}
		return NumberPart.IsEmpty() ? MAX_int32 : FCString::Atoi(*NumberPart); // �������Ȃ���Δ��ɑ傫�Ȑ���Ԃ�
		};

	// �J�X�^����r�֐����g�p���ăt�@�C�������\�[�g
	FileNames.Sort([&](const FString& A, const FString& B) {
		bool bHasNumberA, bHasNumberB;
		int32 NumA = ExtractNumber(A, bHasNumberA);
		int32 NumB = ExtractNumber(B, bHasNumberB);

		if (NumA == NumB) {
			// �������������A�����ɐ������Ȃ��ꍇ�̓A���t�@�x�b�g���ɂ���
			return A < B;
		}
		else {
			// �����Ŕ�r
			return NumA < NumB;
		}
		});


	//�e�t�@�C���Ƀ��[�v����
	for (FString FileName : FileNames) {
		

		//���S�ȃt�@�C���p�X���쐬
		FString FullPath = MaterialDataPath + FileName;
		//�t�@�C������ǂݍ��񂾃e�L�X�g���i�[���邽�߂̕ϐ�
		FString JsonString;
		//UKismetSystemLibrary::PrintString(this, FullPath, true, true, FColor::Cyan, 10.f, TEXT("None"));

		//�t�@�C������e�L�X�g��ǂݍ��݁A���������ꍇ�͏����𑱂���
		if (FFileHelper::LoadFileToString(JsonString, *FullPath)){

			//JSON�I�u�W�F�N�g��ێ����邽�߂̃|�C���^
			TSharedPtr<FJsonObject> JsonObject;
			//JSON���[�_���쐬
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

			//JSON�e�L�X�g���f�V���A���C�Y����JSON�I�u�W�F�N�g�ɕϊ�
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid()) {

				//�}�e���A���\���̃C���X�^���X
				FMaterialProperties Material;

				//�}�e���A���p�����[�^���i�[
				Material.Name = JsonObject->GetStringField(TEXT("name"));
				TArray<TSharedPtr<FJsonValue>> BaseColorArray = JsonObject->GetArrayField(TEXT("base_color.value"));
				Material.BaseColor = FVector(
					BaseColorArray[0]->AsNumber(),
					BaseColorArray[1]->AsNumber(),
					BaseColorArray[2]->AsNumber()
				);
				Material.Metallic = JsonObject->GetNumberField(TEXT("metallic.value"));
				Material.Roughness = JsonObject->GetNumberField(TEXT("roughness.value"));
				Material.Specular = JsonObject->GetNumberField(TEXT("specular"));
				Material.ClearCoat = JsonObject->GetNumberField(TEXT("clearcoat.value"));
				Material.ClearCoatGloss = (1.0f - JsonObject->GetNumberField(TEXT("clearcoat_gloss.value")));
				Material.Matallic_type = JsonObject->GetNumberField(TEXT("matallic_type"));

				MaterialPropertiesArray.Add(Material);

				//UKismetSystemLibrary::PrintString(this, FileName, true, true, FColor::Cyan, 10.f, TEXT("None"));

			}

		}
	}

	//UKismetSystemLibrary::PrintString(this, "json file load", true, true, FColor::Cyan, 10.f, TEXT("None"));
	//�{�^�����쐬
	UPaintWidget::CreateButtonsForMaterials();

}

void UPaintWidget::CreateButtonsForMaterials() {

	for (const FMaterialProperties& Material : MaterialPropertiesArray) {

		UCustomMaterialButton* NewButton = NewObject<UCustomMaterialButton>(this, UCustomMaterialButton::StaticClass());
		if (NewButton) {
			UTextBlock* ButtonLabel = NewObject<UTextBlock>(NewButton, UTextBlock::StaticClass());
			if (ButtonLabel) {
				ButtonLabel->SetText(FText::FromString(Material.Name));
				NewButton->AddChild(ButtonLabel);
			}

			//�J�X�^���{�^�����ƂɃ}�e���A������������
			NewButton->MaterialProperties = Material;
			
			//�N���b�N���ɃJ�X�^���{�^��������OnButtonClicked�𔭉�
			NewButton->OnClicked.AddDynamic(NewButton, &UCustomMaterialButton::OnButtonClicked);

			//�J�X�^���{�^���ɓo�^���Ă���f���Q�[�g����}�e���A���v���p�e�B���󂯎���ăC�x���g���s
			NewButton->OnCustomButtonClicked.AddDynamic(this, &UPaintWidget::PaintChange);

			MyVerticalBox->AddChildToVerticalBox(NewButton);
		}

	}

	//UKismetSystemLibrary::PrintString(this, "generate button", true, true, FColor::Cyan, 10.f, TEXT("None"));
}


void UPaintWidget::PaintChange(const FMaterialProperties& MaterialProperties) {
	

	//���݂̃v���C���[�|�[�����擾
	APawn* PlayerPawn = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn) {
		
		APaintPreview* PaintPreview = Cast<APaintPreview>(PlayerPawn);
		if (PaintPreview) {
			
			UStaticMeshComponent* PreviewModel = Cast<UStaticMeshComponent>(PaintPreview->PreviewModel);
				if (PreviewModel) {
					//UKismetSystemLibrary::PrintString(this, MaterialProperties.Name, true, true, FColor::Cyan, 10.f, TEXT("None"));

					if (MaterialProperties.Matallic_type == 0) {

						//�_�C�i�~�b�N�}�e���A���C���X�^���X���쐬
						UMaterialInstanceDynamic* MID = PreviewModel->CreateDynamicMaterialInstance(MaterialSlot);

						MID->SetScalarParameterValue(TEXT("strength"), 0.f);

						//�p�����[�^�ύX
						MID->SetVectorParameterValue(TEXT("base_color"), MaterialProperties.BaseColor);
						MID->SetScalarParameterValue(TEXT("metallic"), MaterialProperties.Metallic);
						MID->SetScalarParameterValue(TEXT("roughness"), MaterialProperties.Roughness);
						MID->SetScalarParameterValue(TEXT("specular"), MaterialProperties.Specular);
						MID->SetScalarParameterValue(TEXT("clear_coat"), MaterialProperties.ClearCoat);
						MID->SetScalarParameterValue(TEXT("clear_coat_roughness"), (1.0f - MaterialProperties.ClearCoatGloss));

					}

				}
		}
	}
}

void UPaintWidget::ShowHiddenButtons()
{

	for (int32 i = 0; i < HiddenButtons.Num(); ++i) {
		if (HiddenButtons[i]) {
			HiddenButtons[i]->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UPaintWidget::HideAllButtons()
{
	for (UButton* Button : HiddenButtons) {
		if (Button) {
			Button->SetVisibility(ESlateVisibility::Hidden); 
		}
	}
}

void UPaintWidget::OnButtonSphereClicked() {
	HideAllButtons();
	SetMSlot0();
	
	APawn* PlayerPawn = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn) {

		APaintPreview* PaintPreview = Cast<APaintPreview>(PlayerPawn);
		if (PaintPreview) {

			UStaticMeshComponent* PreviewModel = Cast<UStaticMeshComponent>(PaintPreview->PreviewModel);
			if (PreviewModel) {
				
				//�����V�~�����[�V�������I��
				PreviewModel->SetSimulatePhysics(true);

				//���b�V����ݒ�
				UStaticMesh * Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Model/ST_Sphere"));
				PreviewModel->SetStaticMesh(Mesh);

				//PreviewModel�̃X�P�[���𒲐�
				PreviewModel->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

				

			}
		}
	}
}

void UPaintWidget::OnButtonBoxClicked() {
	HideAllButtons();
	SetMSlot0();

	APawn* PlayerPawn = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn) {

		APaintPreview* PaintPreview = Cast<APaintPreview>(PlayerPawn);
		if (PaintPreview) {

			UStaticMeshComponent* PreviewModel = Cast<UStaticMeshComponent>(PaintPreview->PreviewModel);
			if (PreviewModel) {
				//�����V�~�����[�V�������I��
				PreviewModel->SetSimulatePhysics(true);

				//���b�V����ݒ�
				UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
				PreviewModel->SetStaticMesh(Mesh);

				//PreviewModel�̃X�P�[���𒲐�
				PreviewModel->SetRelativeScale3D(FVector(0.38f, 0.38f, 0.38f));


			}
		}
	}
}

void UPaintWidget::OnButtonPlaneClicked() {

	HideAllButtons();
	SetMSlot0();

	APawn* PlayerPawn = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn) {

		APaintPreview* PaintPreview = Cast<APaintPreview>(PlayerPawn);
		if (PaintPreview) {

			UStaticMeshComponent* PreviewModel = Cast<UStaticMeshComponent>(PaintPreview->PreviewModel);
			if (PreviewModel) {

				//�����V�~�����[�V�������I�t
				PreviewModel->SetSimulatePhysics(false);

				//���b�V����ݒ�
				UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane"));
				PreviewModel->SetStaticMesh(Mesh);

				//PreviewModel�̃X�P�[���𒲐�
				PreviewModel->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

				
			}
		}
	}
}

void UPaintWidget::OnButtonMS06RClicked() {

	APawn* PlayerPawn = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn) {

		APaintPreview* PaintPreview = Cast<APaintPreview>(PlayerPawn);
		if (PaintPreview) {

			UStaticMeshComponent* PreviewModel = Cast<UStaticMeshComponent>(PaintPreview->PreviewModel);
			if (PreviewModel) {

				//�ʒu����
				//FVector NewLocation = PlayerPawn->GetActorLocation();
				//NewLocation.Z += 3;
				//PlayerPawn->SetActorLocation(NewLocation);

				//�����V�~�����[�V�������I�t
				PreviewModel->SetSimulatePhysics(false);

				//���b�V����ݒ�
				UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MS06R/Phyco_Zaku2"));
				PreviewModel->SetStaticMesh(Mesh);
				
				//PreviewModel�̃X�P�[���𒲐�
				PreviewModel->SetRelativeScale3D(FVector(0.017f, 0.017f, 0.017f));


				//�B���ꂽ�{�^����\������
				ShowHiddenButtons();

			}
		}
	}
}

void UPaintWidget::SetMSlot0() {
	MaterialSlot = 0;
}

void UPaintWidget::OnButtonParts1Clicked()
{
	MaterialSlot = 0;
}

void UPaintWidget::OnButtonParts2Clicked()
{
	MaterialSlot = 1;
}

void UPaintWidget::OnButtonParts3Clicked()
{
	MaterialSlot = 4;
}

void UPaintWidget::OnButtonParts4Clicked()
{
	MaterialSlot = 5;
}

void UPaintWidget::OnButtonParts5Clicked()
{
	MaterialSlot = 6;
}

void UPaintWidget::OnButtonParts6Clicked()
{
	MaterialSlot = 7;
}
