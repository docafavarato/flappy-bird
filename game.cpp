#include "game.h"
#include <vector>
#include <iostream>

// Inits
void Game::initVariables()
{
	this->window = nullptr;
	this->game_over = false;
	this->points = 0;
	this->add = false;
	this->space = 170.f;
	this->enemySpawnTimerMax = 30.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 4;
	this->gravity = 0.5f;
	this->speed = -3.f;

	this->wing_buffer;
	this->hit_buffer;
	this->point_buffer;
	this->wing_sound;
	this->hit_sound;
	this->point_sound;

}

void Game::initWindow()
{
	this->videoMode.height = 768;
	this->videoMode.width = 432;
	this->window = new sf::RenderWindow(this->videoMode, "Flappy Bird", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initTextures()
{
	int rand_index = rand() % 2;
	std::cout << rand_index;
	switch (rand_index)
	{
		case 0:
			this->bg.loadFromFile("images/background-day.png");
			break;
		case 1:
			this->bg.loadFromFile("images/background-night.png");
			break;
	}
	this->flappy.loadFromFile("images/flappy.png");
	this->base.loadFromFile("images/base.png");
	this->pipe.loadFromFile("images/pipe-green.png");
	this->background.setTexture(this->bg);
	this->background.setScale(1.5f, 1.5f);
}

void Game::initFlappy()
{
	this->bird.setScale(2.f, 2.f);
	this->bird.setTexture(this->flappy);
	this->bird.setPosition(150.f - (this->flappy.getSize().x / 2.f), 400.f - (this->flappy.getSize().y / 2.f));
}

void Game::initEnemies()
{
	this->pipe_bottom.setTexture(this->pipe);
	this->pipe_top.setTexture(this->pipe);
	this->pipe_bottom.setScale(1.5f, 1.5f);
	this->pipe_top.setScale(1.5f, -1.5f);

	this->pipe_bottom.setPosition(200.f, 100.f);
	this->pipe_top.setPosition(100.f, 100.f);

	this->ground.setPosition(0.f, 700.f);
	this->ground.setTexture(this->base);
	this->ground.setScale(1.5f, 1.5f);
}

void Game::initStats()
{
    this->font.loadFromFile("images/flappybird.ttf");

	this->points_text.setOutlineColor(sf::Color::Black);
	this->points_text.setOutlineThickness(3.f);
	this->points_text.setFont(this->font);
	this->points_text.setCharacterSize(50);
	this->points_text.setPosition(216.f, 50.f);

	this->game_over_text.setOutlineColor(sf::Color::Black);
	this->game_over_text.setOutlineThickness(3.f);
	this->game_over_text.setFont(this->font);
	this->game_over_text.setCharacterSize(50);
	this->game_over_text.setString("Game Over");
}

void Game::initSounds()
{
	this->wing_buffer.loadFromFile("audios/wing.wav");
	this->hit_buffer.loadFromFile("audios/hit.wav");
	this->point_buffer.loadFromFile("audios/point.wav");

	this->wing_sound.setBuffer(this->wing_buffer);
	this->hit_sound.setBuffer(this->hit_buffer);
	this->point_sound.setBuffer(this->point_buffer);
}

void Game::gameOver()
{
	this->clearGame();
}
void Game::clearGame()
{
	this->enemies.clear();
	this->points = 0;
	this->initFlappy();
	this->game_over = false;
}

// Updates and Renders
void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		if (this->ev.type == sf::Event::Closed)
		{
			this->window->close();
		}

		if (this->ev.type == sf::Event::KeyPressed)
		{
			if (ev.key.code == sf::Keyboard::Space && this->game_over)
			{
				this->clearGame();
			}
		}
	}
}

void Game::update()
{
	this->pollEvents();
	if (!game_over)
	{
		this->updateFlappy();
		this->updateEnemies();
		this->updateStats();
	}
	
}

void Game::updateFlappy()
{
	this->bird.move(0, this->gravity);
	this->gravity += 0.5f;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->gravity = -7.f;
		this->bird.setRotation(-frame - 10.f);
		this->wing_sound.play();
	}
	else
	{
		this->bird.setRotation(frame - 10.f);
	}
	
	this->frame += 0.15f;
	if (frame > 3)
	{
		frame -= 3;
	}

	this->bird.setTextureRect(sf::IntRect(34 * (int)frame, 0, 34, 24));
}

void Game::updateEnemies()
{
	if (this->enemies.size() < this->maxEnemies)
	{
		this->spawnEnemies();	
		this->maxEnemies = 2;
	}

	for (int i = 0; i < this->enemies.size(); i++)
	{
		bool deleted = false;
		this->enemies[i].move(this->speed, 0.f);

		if (this->enemies[i].getPosition().x < -100)
		{
			deleted = true;
		}

		if (this->enemies[i].getPosition().x == 300)
		{
			this->maxEnemies = 4;
		}

		if (this->enemies[i].getPosition().x == this->bird.getPosition().x)
		{
			if (!add)
			{
				this->points++;
				add = true;
				this->point_sound.play();
			}
			else
			{
				add = false;
			}
		}

		if (this->enemies[i].getGlobalBounds().intersects(this->bird.getGlobalBounds()))
		{
			this->game_over = true;
			this->hit_sound.play();
		}

		if (deleted)
		{
			this->enemies.erase(this->enemies.begin() + i);
		}
	}
}

void Game::updateStats()
{
	this->points_text.setString(std::to_string(this->points));
}

void Game::renderStats()
{
	this->window->draw(this->points_text);
}

void Game::renderFlappy()
{
	this->window->draw(this->background);
	this->window->draw(this->bird);
}

void Game::renderEnemies()
{
	for (auto& e : this->enemies)
	{
		this->window->draw(e);
	}
	this->window->draw(this->ground);
}

void Game::render()
{
	this->window->clear();
	this->renderFlappy();
	this->renderEnemies();
	this->renderStats();

	this->window->display();
}

// Spawns
void Game::spawnEnemies()
{
	int pos = rand() % 275 + 175;
	this->pipe_bottom.setPosition(600.f, pos + space);
	this->pipe_top.setPosition(600.f, pos);

	this->enemies.push_back(this->pipe_bottom);
	this->enemies.push_back(this->pipe_top);
}


Game::Game()
{
	this->initVariables();
	this->initTextures();
	this->initFlappy();
	this->initEnemies();
	this->initStats();
	this->initSounds();
	this->initWindow();
}

Game::~Game()
{
	delete this->window;
}

const bool Game::running() const
{
	return this->window->isOpen();
}