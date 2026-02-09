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
		
		DrawDebugLine(Agent.GetWorld(), AgentPosition, TargetPosition, FColor::Green, false, -1.f, 0, 3.f);
		FVector AgentVelocity = Agent.GetVelocity();
		AgentVelocity.Normalize();
		FVector CurrentDirection = AgentPosition + AgentVelocity * 100.f;
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
		
		DrawDebugLine(Agent.GetWorld(), AgentPosition, TargetPosition, FColor::Green, false, -1.f, 0, 3.f);
		FVector AgentVelocity = Agent.GetVelocity();
		AgentVelocity.Normalize();
		FVector CurrentDirection = AgentPosition + AgentVelocity * 100.f;
		DrawDebugLine(Agent.GetWorld(), AgentPosition, CurrentDirection, FColor::Cyan, false, -1.f, 0, 3.f);
	}
	
	return Steering;
}