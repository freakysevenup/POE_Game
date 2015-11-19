#pragma once
class GameObject
{
public:
	GameObject();
	~GameObject();

	inline glm::vec3 & getPosition() { return m_position; }
	inline glm::vec3 & getScale() { return m_scale; }

protected:
	glm::vec3 m_position;		/* This comes from transform */
	glm::vec3 m_scale;			/* This comes from transform */
	double m_boundingRadius;    /* For collision detection and obstacle avoidance */
};

