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

	// Debug Rendering
	if (Agent.GetDebugRenderingEnabled())
	{
		FVector AgentPosition = FVector(Agent.GetPosition().X, Agent.GetPosition().Y, 0);
		FVector TargetPosition = FVector(Target.Position.X, Target.Position.Y, 0);
		FVector LineToFuture = AgentPosition + FVector(DirectionToFuture.X, DirectionToFuture.Y, 0);

		// Draw Line to future target position
		DrawDebugLine(Agent.GetWorld(), AgentPosition, LineToFuture, FColor::Green, false, -1.f);

		FVector AgentVelocity = Agent.GetVelocity();
		AgentVelocity.Normalize();
		FVector CurrentDirection = AgentPosition + AgentVelocity * 100.f;
		// Draw the current direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, CurrentDirection, FColor::Cyan, false, -1.f, 0, 3.f);
		
		DrawDebugPoint(Agent.GetWorld(), TargetPosition, 10.f, FColor::Red, false, -1.f);
	}
	
	return Steering;
}

SteeringOutput Evade::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering = {};

	float Distance = FVector2D::Distance(Target.Position, Agent.GetPosition());
	float Velocity = Agent.GetMaxLinearSpeed();
	float TimeToReach = Distance / Velocity;

	FVector2D FuturePosition = Target.Position + Target.LinearVelocity * TimeToReach;

	FVector2D DirectionToFuture = (Agent.GetPosition() - FuturePosition);
	DirectionToFuture.Normalize();
	

	Steering.LinearVelocity = DirectionToFuture;

	// Debug Rendering
	if (Agent.GetDebugRenderingEnabled())
	{
		FVector AgentPosition = FVector(Agent.GetPosition().X, Agent.GetPosition().Y, 0);
		FVector TargetPosition = FVector(Target.Position.X, Target.Position.Y, 0);
		FVector LineToFuture = AgentPosition + FVector(DirectionToFuture.X, DirectionToFuture.Y, 0);

		// Draw Line to future target position
		DrawDebugLine(Agent.GetWorld(), AgentPosition, LineToFuture, FColor::Green, false, -1.f);

		FVector AgentVelocity = Agent.GetVelocity();
		AgentVelocity.Normalize();
		FVector CurrentDirection = AgentPosition + AgentVelocity * 100.f;
		// Draw the current direction
		DrawDebugLine(Agent.GetWorld(), AgentPosition, CurrentDirection, FColor::Cyan, false, -1.f, 0, 3.f);
		
		DrawDebugPoint(Agent.GetWorld(), TargetPosition, 10.f, FColor::Red, false, -1.f);
	}
	
	return Steering;
}

SteeringOutput Wander::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
    SteeringOutput Steering{};
    
    float CurrentRotation = Agent.GetRotation();
    FVector2D ForwardDirection = FVector2D(
        FMath::Cos(FMath::DegreesToRadians(CurrentRotation)),
        FMath::Sin(FMath::DegreesToRadians(CurrentRotation))
    );
    
    FVector2D CircleCenter = Agent.GetPosition() + ForwardDirection * WanderDistance;
    
    float RandomAngleOffset = FMath::RandRange(-WanderJitter, WanderJitter);
    WanderAngle += RandomAngleOffset;
    
    WanderAngle = FMath::Fmod(WanderAngle, 360.f);
    
    float AngleInRadians = FMath::DegreesToRadians(WanderAngle);
    FVector2D CircleOffset = FVector2D(
        FMath::Cos(AngleInRadians) * WanderRadius,
        FMath::Sin(AngleInRadians) * WanderRadius
    );
	
    FVector2D WanderTarget = CircleCenter + CircleOffset;
    
    FVector2D DirectionToTarget = WanderTarget - Agent.GetPosition();
    float DistanceToTarget = DirectionToTarget.Size();
    
    if (DistanceToTarget > 0.1f)
    {
        DirectionToTarget.Normalize();
        Steering.LinearVelocity = DirectionToTarget * Agent.GetMaxLinearSpeed();
    }
    
    // Debug Rendering
    if (Agent.GetDebugRenderingEnabled())
    {
        FVector AgentPosition = FVector(Agent.GetPosition().X, Agent.GetPosition().Y, 0);
        FVector CircleCenter3D = FVector(CircleCenter.X, CircleCenter.Y, 0);
        FVector WanderTarget3D = FVector(WanderTarget.X, WanderTarget.Y, 0);
        
        // Draw the wander circle
        DrawDebugCircle(Agent.GetWorld(), CircleCenter3D, WanderRadius, 32, FColor::Blue, false, -1.f, 0, 2.f, FVector(1,0,0), FVector(0,1,0));
        
        // Draw line from agent to circle center
        DrawDebugLine(Agent.GetWorld(), AgentPosition, CircleCenter3D, FColor::Orange, false, -1.f, 0, 2.f);
        
        // Draw line from circle center to wander target
        DrawDebugLine(Agent.GetWorld(), CircleCenter3D, WanderTarget3D, FColor::Purple, false, -1.f, 0, 3.f);
        
        // Draw the wander target point
        DrawDebugPoint(Agent.GetWorld(), WanderTarget3D, 10.f, FColor::Magenta, false, -1.f);
        
        // Draw the current direction
        FVector ForwardLine = AgentPosition + FVector(ForwardDirection.X, ForwardDirection.Y, 0) * (WanderDistance + WanderRadius);
        DrawDebugLine(Agent.GetWorld(), AgentPosition, ForwardLine, FColor::Cyan, false, -1.f, 0, 1.f);
    }
    
    return Steering;
}