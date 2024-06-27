// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleLight.h"

// Sets default values
AToggleLight::AToggleLight()
{
    PrimaryActorTick.bCanEverTick = true;

    UserPointLight1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("UserPointLight1"));
    UserPointLight1->SetupAttachment(RootComponent);
    UserPointLight1->Intensity = 0.0f;
    bIsLight1On = false;

    UserPointLight2 = CreateDefaultSubobject<UPointLightComponent>(TEXT("UserPointLight2"));
    UserPointLight2->SetupAttachment(RootComponent);
    UserPointLight2->Intensity = 0.0f;
    bIsLight2On = false;

    UserPointLight3 = CreateDefaultSubobject<UPointLightComponent>(TEXT("UserPointLight3"));
    UserPointLight3->SetupAttachment(RootComponent);
    UserPointLight3->Intensity = 8000.0f;
    bIsLight3On = true;

    // ポイントライトの位置を設定
    UserPointLight1->SetRelativeLocation(FVector(-911.0f, -477.0f, 190.0f));
    UserPointLight2->SetRelativeLocation(FVector(-692.0f, -477.0f, 190.0f ));
    UserPointLight3->SetRelativeLocation(FVector(-795.0f, -354.0f, 277.0f));
}

// Called when the game starts or when spawned
void AToggleLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToggleLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AToggleLight::ToggleLight(int LightIndex)
{
    switch (LightIndex) {
        case 1:
            bIsLight1On = !bIsLight1On;
            SetLightIntensity(UserPointLight1, LightIndex, bIsLight1On);
            break;
        case 2:
            bIsLight2On = !bIsLight2On;
            SetLightIntensity(UserPointLight2, LightIndex, bIsLight2On);
            break;
        case 3:
            bIsLight3On = !bIsLight3On;
            SetLightIntensity(UserPointLight3, LightIndex, bIsLight3On);
            break;
        default:
            break;
    }
    
}

void AToggleLight::SetLightIntensity(UPointLightComponent* Light,int LightIndex, bool bIsOn)
{

    switch (LightIndex) {
    case 1:
        Light->SetIntensity(bIsOn ? 4000.0f : 0.0f);
        break;
    case 2:
        Light->SetIntensity(bIsOn ? 8000.0f : 0.0f);
        break;
    case 3:
        Light->SetIntensity(bIsOn ? 4000.0f : 0.0f);
        break;
    default:
        break;
    }
}
