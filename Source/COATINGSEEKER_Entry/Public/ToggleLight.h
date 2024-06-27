// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "ToggleLight.generated.h"

UCLASS()
class COATINGSEEKER_ENTRY_API AToggleLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToggleLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* UserPointLight1;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* UserPointLight2;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* UserPointLight3;

	void ToggleLight(int LightIndex);

private:
	void SetLightIntensity(UPointLightComponent* Light, int LightIndex,  bool bIsOn);

	bool bIsLight1On;
	bool bIsLight2On;
	bool bIsLight3On;

};
