// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/ZMQMessageHandle.h"
#include "MessageHandle_SpawnDynamicEntity.generated.h"

/**
 * 
 */
UCLASS()
class LARGESCREENSOLUTION_API UMessageHandle_SpawnDynamicEntity : public UZMQMessageHandle
{
	GENERATED_BODY()

public:
	UMessageHandle_SpawnDynamicEntity();

public:
	virtual void OnReceiveMessage(const FString& InData) override;
};
