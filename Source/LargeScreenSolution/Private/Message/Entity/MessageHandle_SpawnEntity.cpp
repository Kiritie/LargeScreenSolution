// Fill out your copyright notice in the Description page of Project Settings.

#include "Message/Entity/MessageHandle_SpawnEntity.h"

#include "ZMQControlTypes.h"
#include "Scene/SceneManager.h"
#include "Scene/Entity/EntityBase.h"

UMessageHandle_SpawnEntity::UMessageHandle_SpawnEntity()
{
	MessageTag = GameplayTags::Message_SpawnEntity;
}

void UMessageHandle_SpawnEntity::OnReceiveMessage(const FString& InData)
{
	Super::OnReceiveMessage(InData);

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InData);

	TSharedPtr<FJsonObject> JsonObject;
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FString ID = TEXT("");
		UClass* Class = nullptr;
		FVector Location = FVector::ZeroVector;
		FRotator Rotation = FRotator::ZeroRotator;
		FVector Scale = FVector::ZeroVector;
		
		if(JsonObject->HasField(TEXT("id")))
		{
			ID = JsonObject->GetStringField(TEXT("id"));
		}

		if(ASceneManager::Get()->HasSceneObject(ID))
		{
			return;
		}

		if(JsonObject->HasField(TEXT("class")))
		{
			const FString Field = JsonObject->GetStringField(TEXT("class"));
			Class = LoadClass<AActor>(nullptr, *Field);
		}
		if(JsonObject->HasField(TEXT("location")))
		{
			const FString Field = JsonObject->GetStringField(TEXT("location"));
			TArray<FString> Fields;
			if(Field.ParseIntoArray(Fields, TEXT(",")) >= 3)
			{
				Location = FVector(FCString::Atof(*Fields[0]), FCString::Atof(*Fields[1]), FCString::Atof(*Fields[2]));
			}
		}
		if(JsonObject->HasField(TEXT("rotation")))
		{
			const FString Field = JsonObject->GetStringField(TEXT("rotation"));
			TArray<FString> Fields;
			if(Field.ParseIntoArray(Fields, TEXT(",")) >= 3)
			{
				Rotation = FRotator(FCString::Atof(*Fields[0]), FCString::Atof(*Fields[1]), FCString::Atof(*Fields[2]));
			}
		}
		if(JsonObject->HasField(TEXT("scale")))
		{
			const FString Field = JsonObject->GetStringField(TEXT("scale"));
			TArray<FString> Fields;
			if(Field.ParseIntoArray(Fields, TEXT(",")) >= 3)
			{
				Scale = FVector(FCString::Atof(*Fields[0]), FCString::Atof(*Fields[1]), FCString::Atof(*Fields[2]));
			}
		}
		
		if (Class)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AEntityBase* Entity = GetWorld()->SpawnActor<AEntityBase>(Class, SpawnInfo);
			if(Entity)
			{
				Entity->SetActorLocation(Location);
				Entity->SetActorRotation(Rotation);
				Entity->SetActorScale3D(Scale);

				ASceneManager::Get()->AddSceneObject(ID, Entity);
			}
		}
	}
}
