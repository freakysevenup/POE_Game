#include "Agent.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <random>
#include <ctime>

bool Agent::rotateHeadingToFacePosition(glm::vec3 target)
{
	/*

	Vector2D toTarget = Vec2DNormalize(target - m_vPosition);

	double dot = m_vHeading.Dot(toTarget);

	//some compilers lose acurracy so the value is clamped to ensure it
	//remains valid for the acos
	Clamp(dot, -1, 1);

	//first determine the angle between the heading vector and the target
	double angle = acos(dot);

	//return true if the player is facing the target
	if (angle < 0.00001) return true;

	//clamp the amount to turn to the max turn rate
	if (angle > m_dMaxTurnRate) angle = m_dMaxTurnRate;

	//The next few lines use a rotation matrix to rotate the player's heading
	//vector accordingly
	C2DMatrix RotationMatrix;

	//notice how the direction of rotation has to be determined when creating
	//the rotation matrix
	RotationMatrix.Rotate(angle * m_vHeading.Sign(toTarget));
	RotationMatrix.TransformVector2Ds(m_vHeading);
	RotationMatrix.TransformVector2Ds(m_vVelocity);

	//finally recreate m_vSide
	m_vSide = m_vHeading.Perp();

	return false;

	*/

	glm::vec3 toTarget = glm::normalize(target - m_position);

	double dot = glm::dot(m_heading, toTarget);

	//some compilers lose acurracy so the value is clamped to ensure it
	//remains valid for the acos

	glm::clamp(dot, -1.0, 1.0);

	//first determine the angle between the heading vector and the target
	double theta = glm::acos(dot);

	//return true if the player is facing the target
	if (theta < 0.00001)
	{
		return true;
	}

	//clamp the amount to turn to the max turn rate
	if (theta > m_maxTurnRate)
	{
		theta = m_maxTurnRate;
	}

	//The next few lines use a rotation matrix to rotate the player's heading
	//vector accordingly

	glm::mat4 rotationMatrix;

	double rotation = theta * getSignedDistance(m_heading, toTarget);
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			rotationMatrix[i][j] = 0.0f;
		}
	}

	rotationMatrix[0][0] = glm::cos(rotation);
	rotationMatrix[0][1] = glm::sin(rotation);
	rotationMatrix[1][0] = -glm::sin(rotation);
	rotationMatrix[1][1] = glm::cos(rotation);
	rotationMatrix[3][3] = 1.0;
	
	glm::vec4 tempHeading = glm::vec4(m_heading.x, m_heading.y, m_heading.z, 0.0f);
	glm::vec4 tempTarget = glm::vec4(toTarget.x, toTarget.y, toTarget.z, 0.0f);

	glm::vec4 resultHeading = tempHeading * rotationMatrix;
	glm::vec4 resultTarget = tempTarget * rotationMatrix;

	m_heading = glm::vec3(resultHeading.x, resultHeading.y, resultHeading.z);
	toTarget = glm::vec3(resultTarget.x, resultTarget.y, resultTarget.z);

	//finally recreate m_vSide
	m_side = glm::vec3(-m_heading.y, m_heading.x, m_heading.z);

	return false;
}

glm::vec3 Agent::seek(glm::vec3 & target)
{
	glm::vec3 desiredVelocity = glm::normalize(target - m_position) * m_maxSpeed;
	return (desiredVelocity - m_velocity);
}

glm::vec3 Agent::flee(glm::vec3 & target)
{
	glm::vec3 desiredVelocity = glm::normalize(m_position - target) * m_maxSpeed;
	return (desiredVelocity - m_velocity);
}

glm::vec3 Agent::fleeIfInRange(glm::vec3 & target)
{
	const float panicDistanceSquared = panicDist * panicDist;

	float xSeparation = target.x - m_position.x;
	float ySeparation = target.y - m_position.y;
	float distanceSquared = xSeparation * xSeparation + ySeparation * ySeparation;

	//only flee if the target is within 'panic distance'.
	if (distanceSquared > panicDistanceSquared)
	{
		return glm::vec3();
	}
	return flee(target);
}

glm::vec3 Agent::arrive(glm::vec3 & target, Deceleration & deceleration)
{
	glm::vec3 toTarget = target - m_position;

	// calculate the distance to the target position
	float dist = getLength(target);

	if (dist > 0)
	{
		// becasue deceleration is enumerated as an int
		// this value is required tp provide fine tweaking of the deceleration
		const float decelerationTweaker = 0.3f;

		float speed = dist / ((float)deceleration * decelerationTweaker);

		// calculate the speed required to reach the target given the
		// desired deceleration
		speed = glm::min(speed, m_maxSpeed);

		// from here proceed just like seek except we don't
		// need to normalize the toTarget vector because we have already gone to the trouble
		// of calculating its length: dist
		glm::vec3 desiredVelocity = toTarget * speed / dist;

		return (desiredVelocity - m_velocity);
	}
	return glm::vec3();
}

glm::vec3 Agent::pursue(Agent & target)
{
	// if the target is ahead and facing the agent then we can just seek
	// for the target's current position
	glm::vec3 toTarget = target.getPosition() - m_position;

	float relativeHeading = glm::dot(m_heading, target.getHeading());
	if ((glm::dot(toTarget, m_heading) > 0) && (relativeHeading < -0.95f))
	{
		return seek(target.getPosition());
	}

	// otherwise we need to predict where the target will be

	// the look ahead time is proportional to the distance between the evader 
	// and the pursuer; adn is inversely proportional to the sum of the 
	// agent's velocities

	float lookAheadTime = getLength(toTarget) / (m_maxSpeed + target.getSpeed());

	// now look to the predicted future position of the target
	return seek(target.getPosition() + target.getVelocity() * lookAheadTime);
}

glm::vec3 Agent::evade(Agent & target)
{
	// if the target is ahead and facing the agent then we can just seek
	// for the target's current position
	glm::vec3 toTarget = target.getPosition() - m_position;

	// otherwise we need to predict where the target will be

	// the look ahead time is proportional to the distance between the evader 
	// and the pursuer; adn is inversely proportional to the sum of the 
	// agent's velocities

	float lookAheadTime = getLength(toTarget) / (m_maxSpeed + target.getSpeed());

	// now look to the predicted future position of the target
	return flee(target.getPosition() + target.getVelocity() * lookAheadTime);
}

glm::vec3 Agent::findPath()
{
	if (finished())
	{
		clear();
		return glm::vec3();
	}

	float wayPointSeekDistance = 5.0f;
	//move to next target if close enough to current target (working in
	//distance squared space)
	if ((getDistance(getCurrentWayPoint(), m_position)) * (getDistance(getCurrentWayPoint(), m_position))
		< (wayPointSeekDistance)* (wayPointSeekDistance))
	{
		getNextWayPoint();
	}

	if (!finished())
	{
		return seek(getCurrentWayPoint());
	}
	else
	{
		Deceleration decel = normal;
		return arrive(getCurrentWayPoint(), decel);
	}
}

glm::vec3 Agent::offsetPuruit(Agent & leader, glm::vec3 & offset)
{
	/*
	
	//calculate the offset’s position in world space
	SVector2D WorldOffsetPos = PointToWorldSpace(offset,
	leader->Heading(),
	leader->Side(),
	leader->Pos());
	
	*/
	// SVector2D ToOffset = WorldOffsetPos - m_pVehicle->Pos();
	glm::vec3 toOffset = offset - leader.getPosition();

	//the look-ahead time is proportional to the distance between the leader
	//and the pursuer; and is inversely proportional to the sum of both
	//agents’ velocities
	float lookAheadTime = getLength(toOffset) /
		(getMaxSpeed() + leader.getSpeed());
	Deceleration decel = fast;
	//now arrive at the predicted future position of the offset

	return arrive(offset + leader.getVelocity() * lookAheadTime, decel);
	// return Arrive(WorldOffsetPos + leader->Velocity() * LookAheadTime, fast);
}

glm::vec3 Agent::wander()
{
	glm::vec3 targetLocal;
	static std::mt19937 random((unsigned int)time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	m_wanderTarget = m_position;

	m_wanderTarget += glm::vec3(randDir(random) * m_wanderJitter, randDir(random) * m_wanderJitter, randDir(random) * m_wanderJitter);

	// re-project this new vector back onto a unit circle
	glm::normalize(m_wanderTarget);

	// increase the length of the vector to the same
	// as the radius of the wander circle

	m_wanderTarget *= m_wanderRadius;

	// move target into a position m_wanderDist in front of the agent
	// pick a plane to wander on
	/*
	-1 = Y
	0 = X
	1 = Z
	*/
	static std::uniform_real_distribution<int> randPlane(-1, 1);
	int pickPlane = randPlane(random);

	if (pickPlane == 0)
	{
		targetLocal = m_wanderTarget + glm::vec3(m_wanderDistance, 0.0f, 0.0f);
	}
	else if (pickPlane == 1)
	{
		targetLocal = m_wanderTarget + glm::vec3(0.0f, 0.0f, m_wanderDistance);
	}
	else
	{
		targetLocal = m_wanderTarget + glm::vec3(0.0f, m_wanderDistance, 0.0f);
	}

	/*
	project the target into world space
	SVector2D targetWorld = PointToWorldSpace(targetLocal,
		m_pVehicle->Heading(),
		m_pVehicle->Side(),
		m_pVehicle->Pos());
	*/

	return targetLocal - m_position;
	//return targetLocal;
}
	
glm::vec3 Agent::interpose(Agent & A, Agent & B)
{
	//first we need to figure out where the two agents are going to be at
	//time T in the future. This is approximated by determining the time
	//taken to reach the midway point at the current time at max speed.
	glm::vec3 midPoint = (A.getPosition() + B.getPosition()) / 2.0f;

	float timeToReachMidPoint = getDistance(m_position, midPoint) / getMaxSpeed();

	//now we have T, we assume that agent A and agent B will continue on a
	//straight trajectory and extrapolate to get their future positions
	glm::vec3 Apos = A.getPosition() + A.getVelocity() * timeToReachMidPoint;
	glm::vec3 Bpos = B.getPosition() + B.getVelocity() * timeToReachMidPoint;

	//calculate the midpoint of these predicted positions
	midPoint = (Apos + Bpos) / 2.0f;

	// then steer to arrive at it
	Deceleration decel = fast;
	return arrive(midPoint, decel);
}

glm::vec3 Agent::obstacleAvoidance(const std::vector<GameObject *> & obstacles)
{

}