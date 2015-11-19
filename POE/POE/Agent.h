#pragma once
#include "SteeringBehaviours.h"
#include "GameObject.h"

/*

Everything in the POE is an agent that can be manipulated using steering behaviours.
Everything is also a gameobject with a transform that can be rendered.
The only things in the game that are not Agents are the Upgradables (weapons and shields)

*/

const float panicDist = 20.0f;


class Agent 
	: public GameObject, public SteeringBehaviours
{
public:
	Agent() {}
	Agent(
		glm::vec3 position,
		float radius,
		glm::vec3 velocity,
		float maxSpeed,
		glm::vec3 heading,
		float mass,
		glm::vec3 scale,
		float turnRate,
		float maxForce) :
		m_heading(heading),
		m_velocity(velocity),
		m_mass(mass),
		m_side(glm::vec3(-m_heading.y, m_heading.x, m_heading.z)),
		m_maxSpeed(maxSpeed),
		m_maxTurnRate(turnRate),
		m_maxForce(maxForce) 
	{
		m_position = position;
		m_boundingRadius = radius;
		m_scale = scale;
	}


	~Agent();

	// method to add any agent to the scenegraph needed
	// method must be able to call upn a specific node in the 
	// scenegraph

	// Getters
	inline glm::vec3 & getVelocity() { return m_velocity; }
	inline glm::vec3 & getSide() { return m_side; }
	inline glm::vec3 & getHeading() { return m_heading; }
	inline float & getMass() { return m_mass; }
	inline float & getMaxSpeed() { return m_maxSpeed; }
	inline float & getMaxForce() { return m_maxForce; }
	inline float & getSpeed() { return getLength(m_velocity); }
	inline float & getSpeedSq() { return getLengthSq(m_velocity); }
	inline float & getMaxTurnRate() { return m_maxTurnRate; }

	// Setters
	void setVolocity(glm::vec3 & newVel) { m_velocity = newVel; }
	void setMaxSpeed(float & newSpeed) { m_maxSpeed = newSpeed; }
	void setMaxForce(float & newForce) { m_maxForce = newForce; }
	void setHeading(glm::vec3 & newHeading) { m_heading = newHeading; }
	void setMaxTurnRate(float & newTurnRate) { m_maxTurnRate = newTurnRate; }

	bool rotateHeadingToFacePosition(glm::vec3 target);

	virtual glm::vec3 seek(glm::vec3 & target) override;
	virtual glm::vec3 flee(glm::vec3 & target) override;
	virtual glm::vec3 fleeIfInRange(glm::vec3 & target) override;
	virtual glm::vec3 arrive(glm::vec3 & target, Deceleration & deceleration) override;
	virtual glm::vec3 pursue(Agent & target) override;
	virtual glm::vec3 evade(Agent & target) override;
	virtual glm::vec3 findPath() override;
	virtual glm::vec3 wander() override;
	virtual glm::vec3 interpose(Agent & A, Agent & B) override;
	virtual glm::vec3 offsetPuruit(Agent & leader, glm::vec3 & offset) override;
	virtual glm::vec3 obstacleAvoidance(const std::vector<GameObject *> & obstacles) override;

private:

	/* I want these two functions to be in the utility class */
	inline float & getLength(glm::vec3 & vec)
	{
		double value = glm::sqrt(getLengthSq(vec));
		return value;
	}
	inline float & getLengthSq(glm::vec3 & vec)
	{
		double value = ((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
		return value;
	}
	inline float & getSignedDistance(glm::vec3 & me, glm::vec3 & target)
	{
		float clockWise = 1.0f;
		float antiClockWise = -1.0f;

		if ((me.x * target.y) > (me.y * target.y) || (me.z * target.y) > (me.y * target.y))
		{
			return antiClockWise;
		}
		else
		{
			return clockWise;
		}
	}
	inline float & getDistance(glm::vec3 & a, glm::vec3 & b)
	{
		float ySeparation = b.y - a.y;
		float xSeparation = b.x - a.x;

		float rValue = glm::sqrt((ySeparation * ySeparation) + (xSeparation * xSeparation));
		return rValue;
	}

	glm::vec3 m_velocity;

	// A normalized vector pointing in the direction the agent is moving
	glm::vec3 m_heading;

	//  vector perpendicular to the heading vector
	glm::vec3 m_side;

	float m_mass;

	// maximum speed at which this agent may travel
	float m_maxSpeed;

	//maximum force this entity can produce to power itself
	float m_maxForce;

	// the maximum rate in (rads per second) at which this agent can rotate
	float m_maxTurnRate;
};

