#pragma once
#include  "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"
#include <vector>
#include <iostream>

class Game
{
	private:
		sf::RenderWindow* window;
		sf::VideoMode videoMode;
		sf::Event ev;

		// Entities
		std::vector<sf::Sprite> enemies;
		sf::Sprite background, bird, pipe_bottom, pipe_top, ground;

		// Textures and fonts
		sf::Texture bg, flappy, pipe, base;
		sf::Font font;

		// Texts
		sf::Text points_text, game_over_text;

		// Sounds and buffers
		sf::SoundBuffer wing_buffer, hit_buffer, point_buffer;
		sf::Sound wing_sound, hit_sound, point_sound;

		// Functions
		void initVariables();
		void initWindow();
		void initTextures();
		void initFlappy();
		void initEnemies();
		void initStats();
		void initSounds();
		void clearGame();
		void gameOver();

	public:
		Game();
		virtual ~Game();
		void pollEvents();
		void update();
		void render();

		// Functions
		void spawnEnemies();
		void updateFlappy();
		void updateEnemies();
		void updateStats();
		void renderStats();
		void renderFlappy();
		void renderEnemies();

		// Logic
		bool game_over;
		bool add;
		int points;
		int maxEnemies;
		int speed;
		float gravity;
		float space;
		float frame;
		float enemySpawnTimer;
		float enemySpawnTimerMax;

		const bool running() const;
};

