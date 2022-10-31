#pragma once


#include <ctime>
#include <iostream>
#include <vector>
#include <sstream>

#include "Player.h"
#include "SwagBall.h"

class Game
{
private:
	//variable
	sf::VideoMode videoMode;
	sf::RenderWindow* window;
	bool endGame;
	sf::Event event;

	Player player;

	std::vector<SwagBall> swagBalls;
	float spawnTimerMax;
	float spawnTimer;
	int maxSwagBalls;
	int points;

	sf::Font font;
	sf::Text guiText;
	sf::Text endGameText;

	void initWindow();
	void initVariable();
	void initFont();
	void initText();

public:
	//Constructor and Destructor
	Game();
	~Game();

	//Accessory
	const bool& getEndGame() const;
	//Modifiers

	//Function
	const bool running() const;
	void pollEvent();
	
	void spawnSwagBalls();
	const int randBallType() const;

	void updatePlayer();
	void updateGui();
	void update();
	void updateCollision();

	void renderGui(sf::RenderTarget* target);
	void render();

};

