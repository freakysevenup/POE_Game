#pragma once
#include <GLM/glm.hpp>

class SteeringBehaviours
{
public:
	SteeringBehaviours();
	~SteeringBehaviours();

protected:

	enum Deceleration{ slow = 3, normal = 2, fast = 1 };

	virtual glm::vec3 seek(glm::vec3 & target) = 0;
	virtual glm::vec3 flee(glm::vec3 & target) = 0;
	virtual glm::vec3 fleeIfInRange(glm::vec3 & target) = 0;
	virtual glm::vec3 arrive(glm::vec3 & target, Deceleration & deceleration) = 0;
	virtual glm::vec3 pursue(Agent & target) = 0;
	virtual glm::vec3 evade(Agent & target) = 0;
	virtual glm::vec3 findPath() = 0;
	virtual glm::vec3 wander() = 0;
	virtual glm::vec3 interpose(Agent & A, Agent & B) = 0;
	virtual glm::vec3 offsetPuruit(Agent & leader, glm::vec3 & offset) = 0;
	virtual glm::vec3 obstacleAvoidance(const std::vector<GameObject *> & obstacles) = 0;

	void setWayPoint(glm::vec3 & position) { m_wayPoints.push_back(position); }
	void getNextWayPoint() { m_currentWayPoint++; }
	glm::vec3 & getCurrentWayPoint() { return m_wayPoints[m_currentWayPoint]; }
	bool finished() { m_currentWayPoint == m_wayPoints.size() ? true : false; }
	void clear() { m_wayPoints.clear(); m_wayPoints.shrink_to_fit(); }

	float m_wanderRadius;
	float m_wanderDistance;
	float m_wanderJitter;
	float m_dBoxLength;
	float m_minDetectionBoxLength;
	glm::vec3 m_wanderTarget;
	std::vector<glm::vec3> m_wayPoints;
	std::vector<glm::vec3> m_t;
	int m_currentWayPoint = 0;
};

