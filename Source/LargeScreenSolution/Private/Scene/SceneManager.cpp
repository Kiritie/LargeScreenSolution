// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#include "Scene/SceneManager.h"

ASceneManager* ASceneManager::Instance = nullptr;

// Sets default values
ASceneManager::ASceneManager()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));

	SceneObjectMap = TMap<FString, UObject*>();
}

ASceneManager* ASceneManager::Get()
{
	return Instance;
}

void ASceneManager::OnInitialize()
{
	K2_OnInitialize();

	Instance = this;
}

void ASceneManager::OnRefresh(float DeltaSeconds)
{
	K2_OnRefresh(DeltaSeconds);
}

void ASceneManager::OnTermination()
{
	K2_OnTermination();

	Instance = nullptr;
}

void ASceneManager::AddSceneObject(const FString& InID, UObject* InObject)
{
	if(HasSceneObject(InID)) return;

	SceneObjectMap.Add(InID, InObject);
}

void ASceneManager::RemoveSceneObject(const FString& InID)
{
	if(!HasSceneObject(InID)) return;
	
	SceneObjectMap.Remove(InID);
}

void ASceneManager::BeginPlay()
{
	Super::BeginPlay();

	OnInitialize();
}

void ASceneManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnTermination();
}

void ASceneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnRefresh(DeltaTime);
}

bool ASceneManager::HasSceneObject(const FString& InID) const
{
	return SceneObjectMap.Contains(InID);
}

UObject* ASceneManager::GetSceneObject(const FString& InID) const
{
	if(HasSceneObject(InID))
	{
		return SceneObjectMap[InID];
	}
	return nullptr;
}

TArray<UObject*> ASceneManager::GetSceneObjects() const
{
	TArray<UObject*> ReturnValues;
	SceneObjectMap.GenerateValueArray(ReturnValues);
	return ReturnValues;
}
