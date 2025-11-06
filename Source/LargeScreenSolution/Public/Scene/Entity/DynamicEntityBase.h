// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshLoaderStatics.h"
#include "Scene/Entity/EntityBase.h"
#include "DynamicEntityBase.generated.h"

class AMeshLoaderActor;
/**
 * 
 */
UCLASS()
class LARGESCREENSOLUTION_API ADynamicEntityBase : public AEntityBase
{
	GENERATED_BODY()

public:
	virtual void OnInitialize() override;

	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnTermination() override;

public:
	UFUNCTION(BlueprintCallable)
	void LoadMeshAndTextureData(const TArray<FModelData>& ModelData);

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<AMeshLoaderActor*> MeshLoaderActors;
};
