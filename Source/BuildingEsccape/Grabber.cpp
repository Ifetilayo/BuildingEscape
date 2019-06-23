// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetUpInput();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGrabber::SetUpInput()
{
	///Look for attached input componenet
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input) {
		//Bind input axis
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		//move the object we're holding
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

void UGrabber::Grab()
{
	///Try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	///If we hit something then attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation(
		ComponentToGrab,
		NAME_None,
		ActorHit->GetActorLocation(),
		ActorHit->GetActorRotation()
		);
	}
}

//release component after grabbing
void UGrabber::Released()
{
	PhysicsHandle->ReleaseComponent(); 
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Set up query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//Ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		ViewPointLocation,
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return Hit;
}

FVector UGrabber::GetLineTraceEnd() 
{
	GetWorld()->GetFirstPlayerController()->GetActorEyesViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);
	FVector LineTraceDirection = ViewPointRotation.Vector();
	return ViewPointLocation + LineTraceDirection * Reach;
}