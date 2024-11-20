// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"


// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonGenerator::SpawnStartRoom() {
	UE_LOG(LogTemp, Warning, TEXT("RUNNING C++ CLASS"));

	// Spawn first room of the dungeon
	AActor* OriginRoom =  GetWorld()->SpawnActor<AActor>(OriginRoomTemplate, GetRootComponent()->GetComponentLocation(), GetRootComponent()->GetComponentRotation());

	// Append first room's exits to the array of available exits for the dungeon generator to use
	// ExitsArray.Append(OriginRoom->GetComponentsByTag(USceneComponent::StaticClass(), FName("Exits")));

	// for(USceneComponent& Exit : ExitsArray){
		
	// }
}
