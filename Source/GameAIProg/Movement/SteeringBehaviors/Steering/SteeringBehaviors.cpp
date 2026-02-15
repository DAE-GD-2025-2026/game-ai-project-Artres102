#include "SteeringBehaviors.h"
#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"

//SEEK
//*******
// TODO: Do the Week01 assignment :^)

SteeringOutput Seek::CalculateSteering(float DeltaT, ASteeringAgent & Agent)
{
	SteeringOutput Steering{};

	FVector2D TargetDirection = Target.Position - Agent.GetPosition();
	Steering.LinearVelocity = TargetDirection;
	
	TargetDirection.Normalize();

	// Debug Rendering
	if (Agent.GetDebugRenderingEnabled())
	{
		FVector AgentPosition = FVector(Agent.GetPosition().X, Agent.GetPosition().Y, 0);
		TargetDirection.Normalize();
		FVector TargetPosition = AgentPosition + FVector(TargetDirection.X, TargetDirection.Y, 0) * 100.f;
		
		// Draw the desired direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, TargetPosition, FColor::Green, false, -1.f, 0, 3.f);
		FVector AgentVelocity = Agent.GetVelocity();
		AgentVelocity.Normalize();
		FVector CurrentDirection = AgentPosition + AgentVelocity * 100.f;
		//Draw the current direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, CurrentDirection, FColor::Cyan, false, -1.f, 0, 3.f);
	}
	
	return Steering;
}

SteeringOutput Flee::CalculateSteering(float DeltaT, ASteeringAgent & Agent)
{
	SteeringOutput Steering{};

	FVector2D TargetDirection = Agent.GetPosition() - Target.Position;
	Steering.LinearVelocity = TargetDirection;

	TargetDirection.Normalize();
	// Debug Rendering
	if (Agent.GetDebugRenderingEnabled())
	{
		FVector AgentPosition = FVector(Agent.GetPosition().X, Agent.GetPosition().Y, 0);
		TargetDirection.Normalize();
		FVector TargetPosition = AgentPosition + FVector(TargetDirection.X, TargetDirection.Y, 0) * 100.f;

		// Draw the desired direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, TargetPosition, FColor::Green, false, -1.f, 0, 3.f);
		FVector AgentVelocity = Agent.GetVelocity();
		AgentVelocity.Normalize();
		FVector CurrentDirection = AgentPosition + AgentVelocity * 100.f;
		//Draw the current direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, CurrentDirection, FColor::Cyan, false, -1.f, 0, 3.f);
	}
	
	return Steering;
}

SteeringOutput Arrive::CalculateSteering(float DeltaT, ASteeringAgent & Agent)
{
	SteeringOutput Steering{};
	float SlowRadius = 500.f;
	float TargetRadius = 100.f;

	if (OriginalSpeed == -1)
	{
		OriginalSpeed = Agent.GetMaxLinearSpeed();
	}

	FVector2D TargetDirection = Target.Position - Agent.GetPosition();
	Steering.LinearVelocity = TargetDirection;
	float Distance = FVector2D::Distance(Target.Position, Agent.GetPosition());

	UE_LOG(LogTemp, Warning, TEXT("Current Linear Speed: %f"), Agent.GetMaxLinearSpeed());
	
	if (Distance < SlowRadius) Agent.SetMaxLinearSpeed(OriginalSpeed * (Distance / SlowRadius));
	else if (Distance < TargetRadius) Agent.SetMaxLinearSpeed(0.f);
	else Agent.SetMaxLinearSpeed(OriginalSpeed);
	
	// Debug Rendering
	if (Agent.GetDebugRenderingEnabled())
	{
		FVector AgentPosition = FVector(Agent.GetPosition().X, Agent.GetPosition().Y, 0);
		DrawDebugCircle(Agent.GetWorld(), AgentPosition, SlowRadius, 40.f, FColor::Blue, false, -1.f, 0, 3.f, FVector(1,0,0), FVector(0,1,0));
		DrawDebugCircle(Agent.GetWorld(), AgentPosition, TargetRadius, 40.f, FColor::Red, false, -1.f, 0, 3.f, FVector(1,0,0), FVector(0,1,0));

		TargetDirection.Normalize();
		FVector TargetPosition = AgentPosition + FVector(TargetDirection.X, TargetDirection.Y, 0) * 100.f;

		// Draw the desired direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, TargetPosition, FColor::Green, false, -1.f, 0, 3.f);
		FVector AgentVelocity = Agent.GetVelocity();
		AgentVelocity.Normalize();
		FVector CurrentDirection = AgentPosition + AgentVelocity * 100.f;
		//Draw the current direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, CurrentDirection, FColor::Cyan, false, -1.f, 0, 3.f);
	}
	
	return Steering;
}