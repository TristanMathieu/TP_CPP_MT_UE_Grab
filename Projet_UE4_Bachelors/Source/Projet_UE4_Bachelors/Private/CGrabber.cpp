// Fill out your copyright notice in the Description page of Project Settings.


#include "CGrabber.h"
#include "UObject/UnrealType.h"
#include "CollisionQueryParams.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UCGrabber::UCGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//Portée de grab
	fltReach = 500.f;

	//Portée de saisie
	fltPortee = 200.f;

	PhysicsHandle = NULL;

	InputComponent = NULL;
	// ...
}


// Called when the game starts
void UCGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetInputComponent();

	if (InputComponent) {
		
		
			InputComponent->BindAction("Grab", IE_Pressed, this, &UCGrabber::Grab);
			InputComponent->BindAction("Grab", IE_Released, this, &UCGrabber::Release);

	}
	// ...
	
}


// Called every frame
void UCGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetPorteeAttrapage());
	}


	// ...
}

FHitResult UCGrabber::GetFirstPhysicsBodyInReach()
{
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;
	/*
	if(GetWorld()->LineTraceSingleByObjectType(Hit, GetReachLineStart(), GetReachLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters)){

		DrawDebugLine(GetWorld(), GetReachLineStart(), GetReachLineEnd(), FColor::Green, false, 4.f);
	}
	else {
		DrawDebugLine(GetWorld(), GetReachLineStart(), GetReachLineEnd(), FColor::Red, false, 4.f);
	}
	*/
	GetWorld()->LineTraceSingleByObjectType(Hit, GetReachLineStart(), GetReachLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);


	return Hit;
}

void UCGrabber::FindPhysicsHandleComponent()
{

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(ensure(PhysicsHandle))
		UE_LOG(LogTemp,Warning,TEXT("Component PhysicsHandle Found"))
}

void UCGrabber::SetInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (ensure(InputComponent)) {

		UE_LOG(LogTemp, Warning, TEXT("Component Input Found"));
	}
}

void UCGrabber::Handle()
{
	FHitResult Hit = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent *ComponentToGrab = Hit.GetComponent();
	AActor *ActorHit = Hit.GetActor();

	if (ActorHit) {
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation());
	}

}


void UCGrabber::Grab() {
	Handle();
}



void UCGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}



FVector UCGrabber::GetReachLineStart()
{
	FVector PlayerViewPointVector;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointVector, PlayerViewPointRotator);


	return PlayerViewPointVector;
}


//Fonction permettant de récuprerer un objet à une certaine portée
FVector UCGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointVector;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointVector, PlayerViewPointRotator);

	FVector LineEnd = PlayerViewPointVector + PlayerViewPointRotator.Vector() * fltReach;

	return LineEnd;
}




//fonction similaire à getreachlineend, permettant de fixer où le prop est placé devant le joueur
FVector UCGrabber::GetPorteeAttrapage()
{
	FVector PlayerViewPointVector;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointVector, PlayerViewPointRotator);

	FVector LineEnd = PlayerViewPointVector + PlayerViewPointRotator.Vector() * fltPortee;

	return LineEnd;
}