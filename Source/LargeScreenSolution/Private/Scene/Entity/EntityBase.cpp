// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#include "Scene/Entity/EntityBase.h"

// Sets default values
AEntityBase::AEntityBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
}

void AEntityBase::OnInitialize()
{
	K2_OnInitialize();
}

void AEntityBase::OnRefresh(float DeltaSeconds)
{
	K2_OnRefresh(DeltaSeconds);
}

void AEntityBase::OnTermination()
{
	K2_OnTermination();
}

void AEntityBase::BeginPlay()
{
	Super::BeginPlay();

	OnInitialize();
}

void AEntityBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnTermination();
}

void AEntityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnRefresh(DeltaTime);
}
