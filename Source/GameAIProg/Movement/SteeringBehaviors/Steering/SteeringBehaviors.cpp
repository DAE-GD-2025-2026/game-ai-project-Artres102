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
		FVector TargetLinePosition = AgentPosition + FVector(TargetDirection.X, TargetDirection.Y, 0) * 100.f;
		FVector TargetPosition = FVector(Target.Position.X, Target.Position.Y, 0);
		
		// Draw the desired direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, TargetLinePosition, FColor::Green, false, -1.f, 0, 3.f);
		FVector AgentVelocity = Agent.GetVelocity();
		AgentVelocity.Normalize();
		FVector CurrentDirection = AgentPosition + AgentVelocity * 100.f;
		// Draw the current direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, CurrentDirection, FColor::Cyan, false, -1.f, 0, 3.f);

		// Draw Target
		DrawDebugPoint(Agent.GetWorld(), TargetPosition, 10.f, FColor::Red, false, -1.f);
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
		FVector TargetLinePosition = AgentPosition + FVector(TargetDirection.X, TargetDirection.Y, 0) * 100.f;
		FVector TargetPosition = FVector(Target.Position.X, Target.Position.Y, 0);

		// Draw the desired direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, TargetLinePosition, FColor::Green, false, -1.f, 0, 3.f);
		FVector AgentVelocity = Agent.GetVelocity();
		AgentVelocity.Normalize();
		FVector CurrentDirection = AgentPosition + AgentVelocity * 100.f;
		//Draw the current direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, CurrentDirection, FColor::Cyan, false, -1.f, 0, 3.f);

		// Draw Target
		DrawDebugPoint(Agent.GetWorld(), TargetPosition, 10.f, FColor::Red, false, -1.f);
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
		FVector TargetLinePosition = AgentPosition + FVector(TargetDirection.X, TargetDirection.Y, 0) * 100.f;
		FVector TargetPosition = FVector(Target.Position.X, Target.Position.Y, 0);

		// Draw the desired direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, TargetLinePosition, FColor::Green, false, -1.f, 0, 3.f);
		FVector AgentVelocity = Agent.GetVelocity();
		AgentVelocity.Normalize();
		FVector CurrentDirection = AgentPosition + AgentVelocity * 100.f;
		// Draw the current direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, CurrentDirection, FColor::Cyan, false, -1.f, 0, 3.f);

		// Draw Target
		DrawDebugPoint(Agent.GetWorld(), TargetPosition, 10.f, FColor::Red, false, -1.f);
	}
	
	return Steering;
}

SteeringOutput Face::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
    SteeringOutput Steering{};

    FVector2D TargetDirection = Target.Position - Agent.GetPosition();

    if (TargetDirection.IsNearlyZero())
    {
        return Steering;
    }

    float TargetRotation = FMath::RadiansToDegrees(FMath::Atan2(TargetDirection.Y, TargetDirection.X));

    float CurrentRotation = Agent.GetRotation(); 
    float DeltaRotation = FMath::FindDeltaAngleDegrees(CurrentRotation, TargetRotation);
    if (FMath::Abs(DeltaRotation) > 0.5f) 
    {
        float MaxAngularSpeed = Agent.GetMaxAngularSpeed();
        Steering.AngularVelocity = DeltaRotation * 10.f; 
        Steering.AngularVelocity = FMath::Clamp(Steering.AngularVelocity, -MaxAngularSpeed, MaxAngularSpeed);
    }
    else
    {
        Steering.AngularVelocity = 0.f;
    }
    Steering.LinearVelocity = FVector2D::ZeroVector;

	// Debug Rendering
	if (Agent.GetDebugRenderingEnabled())
	{
		FVector AgentPosition = FVector(Agent.GetPosition().X, Agent.GetPosition().Y, 0);
		TargetDirection.Normalize();
		FVector TargetLinePosition = AgentPosition + FVector(TargetDirection.X, TargetDirection.Y, 0) * 100.f;
		FVector TargetPosition = FVector(Target.Position.X, Target.Position.Y, 0);

		// Draw the desired direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, TargetLinePosition, FColor::Green, false, -1.f, 0, 3.f);
		FVector AgentVelocity = Agent.GetVelocity();
		AgentVelocity.Normalize();
		FVector CurrentDirection = AgentPosition + AgentVelocity * 100.f;
		// Draw the current direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, CurrentDirection, FColor::Cyan, false, -1.f, 0, 3.f);

		// Draw Target
		DrawDebugPoint(Agent.GetWorld(), TargetPosition, 10.f, FColor::Red, false, -1.f);
	}
    
    return Steering;
}

SteeringOutput Pursuit::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};

	float Distance = FVector2D::Distance(Target.Position, Agent.GetPosition());
	float Velocity = Agent.GetMaxLinearSpeed();
	float TimeToReach = Distance / Velocity;

	FVector2D FuturePosition = Target.Position + Target.LinearVelocity * TimeToReach;

	FVector2D DirectionToFuture = (FuturePosition - Agent.GetPosition());
	DirectionToFuture.Normalize();
	

	Steering.LinearVelocity = DirectionToFuture;
	
	return Steering;
}