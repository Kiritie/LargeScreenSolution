// Fill out your copyright notice in the Description page of Project Settings.

#include "Message/Sound/MessageHandle_PlaySound.h"

#include "ZMQControlTypes.h"
#include "Components/AudioComponent.h"
#include "Scene/SceneManager.h"

UMessageHandle_PlaySound::UMessageHandle_PlaySound()
{
	MessageTag = GameplayTags::Message_PlaySound;
}

void UMessageHandle_PlaySound::OnReceiveMessage(const FString& InData)
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

		UAudioComponent* AudioComponent = ASceneManager::Get()->GetSceneObject<UAudioComponent>(ID);
		if(AudioComponent)
		{
			AudioComponent->DestroyComponent();

			ASceneManager::Get()->RemoveSceneObject(ID);
		}
	}
}
