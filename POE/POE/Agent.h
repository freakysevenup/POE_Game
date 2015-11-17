#pragma once
#include "SteeringBehaviours.h"
#include "GameObject.h"

/*

Everything in the POE is an agent that can be manipulated using steering behaviours.
Everything is also a gameobject with a transform that can be rendered.
The only things in the game that are not Agents are the Upgradables (weapons and shields)

*/
class Agent 
	: public GameObject, public SteeringBehaviours
{
public:
	Agent();
	~Agent();

	// method to add any agent to the scenegraph needed
	// method must be able to call upn a specific node in the 
	// scenegraph

private:

};

