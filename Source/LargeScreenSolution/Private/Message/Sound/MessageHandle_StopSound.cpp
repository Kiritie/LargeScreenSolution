// Fill out your copyright notice in the Description page of Project Settings.

#include "Message/Sound/MessageHandle_StopSound.h"

#include "ZMQControlTypes.h"
#include "Components/AudioComponent.h"
#include "Scene/SceneManager.h"

UMessageHandle_StopSound::UMessageHandle_StopSound()
{
	MessageTag = GameplayTags::Message_StopSound;
}

void UMessageHandle_StopSound::OnReceiveMessage(const FString& InData)
{
	Super::OnReceiveMessage(InData);

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InData);

	TSharedPtr<FJsonObject> JsonObject;
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FString ID = TEXT("");
		
		if(JsonObject->HasField(TEXT("id")))
		{
			ID = JsonObject->GetStringField(TEXT("id"));
		}

		if(ASceneManager::Get()->HasSceneObject(ID))
		{
			return;
		}

		UAudioComponent* AudioComponent = ASceneManager::Get()->GetSceneObject<UAudioComponent>(ID);
		if(AudioComponent)
		{
			AudioComponent->DestroyComponent();

			ASceneManager::Get()->RemoveSceneObject(ID);
		}
	}
}
