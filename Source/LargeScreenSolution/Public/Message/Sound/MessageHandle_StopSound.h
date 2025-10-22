// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/ZMQMessageHandle.h"
#include "MessageHandle_StopSound.generated.h"

/**
 * 
 */
UCLASS()
class LARGESCREENSOLUTION_API UMessageHandle_StopSound : public UZMQMessageHandle
{
	GENERATED_BODY()

public:
	UMessageHandle_StopSound();

public:
	virtual void OnReceiveMessage(const FString& InData) override;
};
