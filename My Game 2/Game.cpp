#include "Game.h"

void Game::initWindow()
{
	this->videoMode = sf::VideoMode(800, 600);
	this->window = new sf::RenderWindow(this->videoMode, "Game 2", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(60);
}

void Game::initVariable()
{
	this->endGame = false;
	this->spawnTimerMax = 10.f;
	this->spawnTimer = this->spawnTimerMax;
	this->maxSwagBalls = 10;
	this->points = 0;
}

void Game::initFont()
{
	if (!this->font.loadFromFile("Fonts/PixellettersFull.ttf")) {
		std::cout << " ! ERROR GAME INITFONT COULD NOT LOAD PixellettersFull.ttf" << "\n";
	}
}

void Game::initText()
{
	this->guiText.setFont(this->font);
	this->guiText.setFillColor(sf::Color(255,255,255,255));
	this->guiText.setCharacterSize(32);

	this->endGameText.setFont(this->font);
	this->endGameText.setFillColor(sf::Color(255, 0, 0));
	this->endGameText.setCharacterSize(60);
	this->endGameText.setPosition(sf::Vector2f(50, 240));
	this->endGameText.setString("YOU ARE DEAD, EXIT THE GAME!");
}

//Contructor and Destructor
Game::Game()
{
	this->initVariable();
	this->initWindow();
	this->initFont();
	this->initText();
}

Game::~Game()
{
	delete this->window;
}

const bool& Game::getEndGame() const
{
	return this->endGame;
}

const bool Game::running() const
{
	return this->window->isOpen() /* && this->endGame == false*/;
}

void Game::pollEvent()
{
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type) {
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->event.key.code == sf::Keyboard::Escape) {
				this->window->close();
			}
			break;
		}
	}
}

void Game::spawnSwagBalls()
{
	if (this->spawnTimer < this->spawnTimerMax) {
		this->spawnTimer += 1.f;
	}
	else {
		if (this->swagBalls.size() < this->maxSwagBalls) {
			this->swagBalls.push_back(SwagBall(*this->window, this->randBallType()));
			this->spawnTimer = 0.f;
		}
	}
}

const int Game::randBallType() const
{
	int type = SwagBallType::DEFAULT;

	int randValue = rand() % 100 + 1;
	if (randValue > 60 && randValue <= 80) {
		type = SwagBallType::DAMAGING;
	}
	else if (randValue > 80 && randValue <= 100) {
		type = SwagBallType::HEALING;
	}
	return type;
}

//Function
void Game::updateCollision()
{
	for (size_t i = 0; i < this->swagBalls.size(); i++) {
		if (this->player.getShape().getGlobalBounds().intersects(this->swagBalls[i].getShape().getGlobalBounds())) {
			switch (this->swagBalls[i].getType())
			{
			case SwagBallType::DEFAULT:
				this->points++;
				break;
			case SwagBallType::DAMAGING:
				this->player.takeDamage(5);
				break;
			case SwagBallType::HEALING:
				this->player.gainHealth(1);
				this->points++;
				break;
			}

			//this->points++;

			this->swagBalls.erase(this->swagBalls.begin() + i);
			
		}
	}

}

void Game::updatePlayer()
{
	this->player.update(this->window);

	if (this->player.getHp() <= 0) {
		this->endGame = true;
	}
}

void Game::updateGui()
{
	std::stringstream ss;

	ss << "Point : " << this->points << "\n"
		<< "Health : " << this->player.getHp() << " / " << this->player.getHpMax() << "\n";

	this->guiText.setString(ss.str());
}

void Game::update()
{
	this->pollEvent();

	if (this->endGame == false) {
		this->spawnSwagBalls();
		this->updatePlayer();
		this->updateCollision();
		this->updateGui();
	}	
}

void Game::renderGui(sf::RenderTarget* target)
{
	target->draw(this->guiText,sf::RenderStates());
}

void Game::render()
{
	this->window->clear();

	this->player.render(this->window);

	for (auto i : this->swagBalls) {
		i.render(*this->window);
	}

	this->renderGui(this->window);

	if (this->endGame == true) {
		this->window->draw(this->endGameText,sf::RenderStates());
	}
	this->window->display();
}