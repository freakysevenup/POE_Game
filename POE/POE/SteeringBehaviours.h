#pragma once
class SteeringBehaviours
{
public:
	SteeringBehaviours();
	~SteeringBehaviours();

protected:

	glm::vec3 pursue(Transform & transform);
	glm::vec3 evade(Transform & transform);
	glm::vec3 avoid(Transform & transform);
	glm::vec3 findPath(Transform & transform, GameObject root);
	glm::vec3 wander(Transform & transform);
};

