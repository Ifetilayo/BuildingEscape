// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//How far ahead the player can reach in cm
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* Input = nullptr;

	//Ray-cast and grab
	void Grab();
	void Released();

	//Find attached physics handle
	void FindPhysicsHandleComponent();

	//set up input component
	void SetUpInput();

	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	
	FVector ViewPointLocation;
	FRotator ViewPointRotation;

	//returns current end of reach line
	FVector GetLineTraceEnd();
};

