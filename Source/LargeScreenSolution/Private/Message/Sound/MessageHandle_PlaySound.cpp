// Fill out your copyright notice in the Description page of Project Settings.

#include "Message/Sound/MessageHandle_PlaySound.h"

#include "ZMQControlTypes.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Scene/SceneManager.h"
#include "Scene/Entity/EntityBase.h"

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
		FString EntityID = TEXT("");
		USoundBase* Sound = nullptr;
		float Volume = 0.f;
		
		if(JsonObject->HasField(TEXT("id")))
		{
			ID = JsonObject->GetStringField(TEXT("id"));
		}
		if(JsonObject->HasField(TEXT("entity_id")))
		{
			EntityID = JsonObject->GetStringField(TEXT("entity_id"));
		}
		if(JsonObject->HasField(TEXT("sound")))
		{
			const FString Field = JsonObject->GetStringField(TEXT("sound"));
			Sound = LoadObject<USoundBase>(nullptr, *Field);
		}
		if(JsonObject->HasField(TEXT("volume")))
		{
			Volume = JsonObject->GetNumberField(TEXT("volume"));
		}

		if(Sound)
		{
			UAudioComponent* AudioComponent = nullptr;
			if(EntityID.IsEmpty())
			{
				AudioComponent = UGameplayStatics::SpawnSound2D(this, Sound, Volume, 1.f, 0.f, nullptr, nullptr, false);
			}
			else
			{
				AEntityBase* Entity = ASceneManager::Get()->GetSceneObject<AEntityBase>(EntityID);
				if(Entity)
				{
					AudioComponent = UGameplayStatics::SpawnSoundAttached(Sound, Entity->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false, Volume, 1.f, 0.f, nullptr, nullptr, false);
				}
			}

			ASceneManager::Get()->AddSceneObject(ID, AudioComponent);
		}
	}
}
