// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Get the start location of the platform when game starts
	StartLocation = GetActorLocation();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);

	RotatePlatform(DeltaTime);
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	// *** SEND PLATFORM BACK IF GONE TO FAR ***
		// Reverse direction of motion if gone too far
	if(ShouldPlatformReturn())
	{
		// Fix to prevent platform overshooting and drifting bug:
		// A better approach than setting StartLocation = CurrentLocation:
		// Prevent start location from drifting over time. If the platform
		// velocity is set too high in the UI - it will cause the 
		// DistanceMoved to be greater than the MoveDistance

		// GetSafeNormal gets the DIRECTION of the Velocity arrow
		FVector MoveDirection = PlatformVelocity.GetSafeNormal();
		StartLocation = StartLocation + MoveDirection * MoveDistance;
		SetActorLocation(StartLocation);
		PlatformVelocity = -PlatformVelocity;
	}
	else
	{
		// *** MOVE THE PLATFORM FORWARD ***
			// Get current location
		FVector CurrentLocation = GetActorLocation();
			// Add vector to that location
			// *Use Delta Time to make the platform move on different computers regardless of 
			// CPU performance / framerate
		CurrentLocation = CurrentLocation + (PlatformVelocity * DeltaTime);
			// Set location
		SetActorLocation(CurrentLocation);
	}	
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	//UE_LOG(LogTemp, Display, TEXT("%s Rotating... "), *GetName());

	// FRotator CurrentRotation = GetActorRotation();
	// CurrentRotation = CurrentRotation + RotationVelocity * DeltaTime;
	// SetActorRotation(CurrentRotation);
	AddActorLocalRotation(RotationVelocity * DeltaTime);
}

bool AMovingPlatform::ShouldPlatformReturn() const
{
		// Log the Overshot distance
	// float OverShoot = DistanceMoved - MoveDistance;
	// FString Name = GetName();
	// UE_LOG(LogTemp, Display, TEXT("Platform %s overshot by:  %f"), *Name, OverShoot);

	return GetDistanceMoved() > MoveDistance;
}

float AMovingPlatform::GetDistanceMoved() const
{
		// Check how far we've moved
	return FVector::Dist(StartLocation, GetActorLocation());
}
