#include "Application.h"


Application::Application()
{
}


Application::~Application()
{
}

void Application::run()
{
	// initialize window and camera


	init();
}

void Application::init()
{
	// initialize game agents and systems

	update();
}

void Application::update()
{
	while (true)
	{
		// update window
		// swapbuffers


		// do stuff

		draw();
		processInput();
	}
}

void Application::draw()
{
	// draw the agents in the world
}

void Application::processInput()
{
	// check input
}
