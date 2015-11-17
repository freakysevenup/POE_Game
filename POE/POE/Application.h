#pragma once 
#include <vector>
#include <string>

class Agent;
class Upgradable;

enum GameState
{
	SPLASH,
	RUNNING,
	PAUSE_MENU,
	MAIN_MENU,
	MISSION_SELECT_MENU,
	MISSION_END_MENU,
	GAME_OVER_MENU,
	GAME_END_SCREEN,
	SAVE,
	EXIT
};

class Application
{
public:
	Application();
	~Application();

	void run();
	void init();

private:

	void update();
	void draw();
	void processInput();

	GameState m_gameState = GameState::SPLASH;
};

