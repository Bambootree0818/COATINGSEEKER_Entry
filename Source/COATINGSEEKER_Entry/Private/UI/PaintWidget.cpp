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
#include "Engine/Texture2D.h"
#include "Brushes/SlateImageBrush.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Image.h"
#include <ToggleLight.h>
#include "Kismet/KismetSystemLibrary.h"

class UVerticalBox;

void UPaintWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ButtonSphereのOnClickedに「OnButtonSphereClicked」を関連づける
	Sphere->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonSphereClicked);

	// ButtonBoxのOnClickedに「OnButtonBoxClicked」を関連づける
	Box->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonBoxClicked);

	// ButtonPlaneのOnClickedに「OnButtonPlaneClicked」を関連づける
	Plane->OnClicked.AddUniqueDynamic(this, &UPaintWidget::OnButtonPlaneClicked);

	// ButtonMS06RのOnClickedに「OnButtonMS06RClicked」を関連づける
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

	if (UserLight1Button)
	{
		UserLight1Button->OnClicked.AddDynamic(this, &UPaintWidget::OnUserLight1ButtonClicked);
	}

	if (UserLight2Button)
	{
		UserLight2Button->OnClicked.AddDynamic(this, &UPaintWidget::OnUserLight2ButtonClicked);
	}

	if (UserLight3Button)
	{
		UserLight3Button->OnClicked.AddDynamic(this, &UPaintWidget::OnUserLight3ButtonClicked);
	}

	//ゲーム終了ボタンのバインド
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPaintWidget::OnQuitButtonClicked);
	}

}


void UPaintWidget::LoadMaterialData() {

	//Contentディレクトリのパスを取得
	FString BasePath = FPaths::ProjectContentDir();

	FString MaterialDataPath = BasePath + TEXT("MaterialDatas/");

	//UKismetSystemLibrary::PrintString(this, MaterialDataPath, true, true, FColor::Cyan, 10.f, TEXT("None"));

	//ファイルマネージャーのインスタンスを取得
	IFileManager& FileManager = IFileManager::Get();

	//読み込むファイル名の配列
	TArray<FString> FileNames;

	//指定パスからJSONファイルのリストを取得
	FileManager.FindFiles(FileNames, *MaterialDataPath, *FString("*.json"));

	// ファイル並び替え
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
		return NumberPart.IsEmpty() ? MAX_int32 : FCString::Atoi(*NumberPart); // 数字がなければ非常に大きな数を返す
		};

	// カスタム比較関数を使用してファイル名をソート
	FileNames.Sort([&](const FString& A, const FString& B) {
		bool bHasNumberA, bHasNumberB;
		int32 NumA = ExtractNumber(A, bHasNumberA);
		int32 NumB = ExtractNumber(B, bHasNumberB);

		if (NumA == NumB) {
			// 数字が同じか、両方に数字がない場合はアルファベット順にする
			return A < B;
		}
		else {
			// 数字で比較
			return NumA < NumB;
		}
		});


	//各ファイルにループ処理
	for (FString FileName : FileNames) {
		

		//完全なファイルパスを作成
		FString FullPath = MaterialDataPath + FileName;
		//ファイルから読み込んだテキストを格納するための変数
		FString JsonString;
		//UKismetSystemLibrary::PrintString(this, FullPath, true, true, FColor::Cyan, 10.f, TEXT("None"));

		//ファイルからテキストを読み込み、成功した場合は処理を続ける
		if (FFileHelper::LoadFileToString(JsonString, *FullPath)){

			//JSONオブジェクトを保持するためのポインタ
			TSharedPtr<FJsonObject> JsonObject;
			//JSONリーダを作成
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

			//JSONテキストをデシリアライズしてJSONオブジェクトに変換
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid()) {

				//マテリアル構造体インスタンス
				FMaterialProperties Material;

				//マテリアルパラメータを格納
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
	//ボタンを作成
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

			// 画像の取得と設定
			FString ImagePath = FString::Printf(TEXT("/Game/UI/MaterialImage/%s"), *Material.Name);
			//UKismetSystemLibrary::PrintString(this, ImagePath, true, true, FColor::Cyan, 20.f, TEXT("None"));
			UTexture2D* ButtonImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *ImagePath));
			if (ButtonImage) {
				//UKismetSystemLibrary::PrintString(this, ImagePath, true, true, FColor::Cyan, 20.f, TEXT("None"));
				FSlateBrush Brush;
				Brush.SetResourceObject(ButtonImage);
				Brush.TintColor = FSlateColor(FLinearColor::White);

				// 通常時の画像を設定
				NewButton->WidgetStyle.Normal.SetResourceObject(ButtonImage);
				NewButton->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor::White);
			}

			//カスタムボタンごとにマテリアルを持たせる
			NewButton->MaterialProperties = Material;
			
			//クリック時にカスタムボタンが持つOnButtonClickedを発火
			NewButton->OnClicked.AddDynamic(NewButton, &UCustomMaterialButton::OnButtonClicked);

			//カスタムボタンに登録しているデリゲートからマテリアルプロパティを受け取ってイベント実行
			NewButton->OnCustomButtonClicked.AddDynamic(this, &UPaintWidget::PaintChange);

			MyVerticalBox->AddChildToVerticalBox(NewButton);
		}

	}

	
}


void UPaintWidget::PaintChange(const FMaterialProperties& MaterialProperties) {
	

	//現在のプレイヤーポーンを取得
	APawn* PlayerPawn = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn) {
		
		APaintPreview* PaintPreview = Cast<APaintPreview>(PlayerPawn);
		if (PaintPreview) {
			
			UStaticMeshComponent* PreviewModel = Cast<UStaticMeshComponent>(PaintPreview->PreviewModel);
				if (PreviewModel) {
					//UKismetSystemLibrary::PrintString(this, MaterialProperties.Name, true, true, FColor::Cyan, 10.f, TEXT("None"));

					if (MaterialProperties.Matallic_type == 0) {

						//ダイナミックマテリアルインスタンスを作成
						UMaterialInstanceDynamic* MID = PreviewModel->CreateDynamicMaterialInstance(MaterialSlot);

						MID->SetScalarParameterValue(TEXT("strength"), 0.f);

						//パラメータ変更
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
				
				//物理シミュレーションをオン
				PreviewModel->SetSimulatePhysics(true);

				//メッシュを設定
				UStaticMesh * Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Model/ST_Sphere"));
				PreviewModel->SetStaticMesh(Mesh);

				//PreviewModelのスケールを調整
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
				//物理シミュレーションをオン
				PreviewModel->SetSimulatePhysics(true);

				//メッシュを設定
				UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
				PreviewModel->SetStaticMesh(Mesh);

				//PreviewModelのスケールを調整
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

				//物理シミュレーションをオフ
				PreviewModel->SetSimulatePhysics(false);

				//メッシュを設定
				UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane"));
				PreviewModel->SetStaticMesh(Mesh);

				//PreviewModelのスケールを調整
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

				//位置調整
				//FVector NewLocation = PlayerPawn->GetActorLocation();
				//NewLocation.Z += 3;
				//PlayerPawn->SetActorLocation(NewLocation);

				//物理シミュレーションをオフ
				PreviewModel->SetSimulatePhysics(false);

				//メッシュを設定
				UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MS06R/Phyco_Zaku2"));
				PreviewModel->SetStaticMesh(Mesh);
				
				//PreviewModelのスケールを調整
				PreviewModel->SetRelativeScale3D(FVector(0.017f, 0.017f, 0.017f));


				//隠されたボタンを表示する
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


void UPaintWidget::OnUserLight1ButtonClicked()
{
	ToggleLight(1);
}

void UPaintWidget::OnUserLight2ButtonClicked()
{
	ToggleLight(2);
}

void UPaintWidget::OnUserLight3ButtonClicked()
{
	ToggleLight(3);
}

void UPaintWidget::ToggleLight(int LightIndex)
{
	// Find all instances of AMyLightActor in the world
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AToggleLight::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AToggleLight* LightActor = Cast<AToggleLight>(Actor);
		if (LightActor)
		{
			LightActor->ToggleLight(LightIndex);
		}
	}
}

void UPaintWidget::OnQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}