#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <map>

using namespace std;

class Bullet {
private:
	float velocity;
	sf::Vector2f direction;
	sf::CircleShape caliber;

public:
	Bullet(sf::Color color) {
		velocity = 0.7f;
		caliber.setRadius(5.0f);
		caliber.setFillColor(color);
		caliber.setScale(0.5f, 0.5f);

	}
	void setDirection(sf::Vector2f direction) {
		if (direction.x > 0) this->direction.x = 1;
		else if (direction.x == 0) this->direction.x = 0;
		else this->direction.x = -1;

		if (direction.y > 0) this->direction.y = 1;
		else if (direction.y == 0) this->direction.y = 0;
		else this->direction.y = -1;

	}
	sf::CircleShape getCircleShape() {
		return  caliber;
	}

	void setPosition(sf::Vector2f position) {
		caliber.setPosition(position);
	}

	sf::Vector2f getPosition() {
		return caliber.getPosition();
	}
	void move() {
		caliber.move(velocity*direction);
	}
};

class Trooper { //Inheritor
protected:
	sf::Sprite sprite;
	vector<Bullet*> bullets;
public:
	bool checkHit(Trooper* enemy) {
		for (int i = 0; i < bullets.size(); i++) {
			if (bullets[i]->getCircleShape().getGlobalBounds().intersects(enemy->sprite.getGlobalBounds())) {
				return true;
			}
		}
		return false;
	}

	void updateBullets() {
		for (int i = 0; i < bullets.size(); i++) {
			int x = bullets[i]->getCircleShape().getPosition().x;
			int y = bullets[i]->getCircleShape().getPosition().y;
			if (x < 0 || x >= 1920 || y < 0 || y >= 1080) {
				bullets.erase(bullets.begin() + i);
			}
		}
	}

};
class PlayerSoldier : public Trooper{

private:
	int respawns;
	int score;
	int kills;
	int deaths;
	float speed;
	Bullet* bullet;
	sf::Vector2f direction; //Where the player is facing
	sf::Vector2f movement;
	sf::Vector2f position;
	sf::Texture soldierTexture;
	sf::RenderWindow*window;

public: 
	
	PlayerSoldier(sf::RenderWindow*window) { //constructor

		this->window = window;
		soldierTexture.loadFromFile("rifleman.png");
		sprite.setTexture(soldierTexture);
		sprite.setPosition(960, 540);
		sprite.setOrigin(98, 121);
		sprite.setScale(0.3f, 0.3f);
		direction.x = 1;// Where the player is facing in the x direction
		direction.y = 0; // where player is facing in the y direction
		speed = .5f;
	}

	void move(sf::Vector2f& movement) {
		sprite.move(movement);
		for (int i = 0; i < bullets.size(); i++) {
			bullets[i]->move();
		}
	}

	void shoot() {
		bullet = new Bullet (sf::Color::White);
	    bullet->setDirection(direction);
		bullet->setPosition(sprite.getPosition());
		//cout << "Direction: " << direction.x << "," << direction.y << endl;
		//cout << "Position: " << soldierSprite.getPosition().x << "," << soldierSprite.getPosition().y << endl;
		bullets.push_back(bullet);

	}

	void die() {
		cout << "You have died on the battlefield....." << endl;
		//exit(0);
	}

	sf::Sprite getSoldierSprite() {
		return sprite;
	}

	void draw() {
	    window->draw(sprite);
		for (int i = 0; i < bullets.size(); i++) {
			window->draw(bullets[i]->getCircleShape());
		}
	}

	void changeDirection(sf::Vector2f direction) {
		if (direction.x == 0 && direction.y == 0) return;
		int x = direction.x;
		int y = direction.y;

		this->direction = direction;

		if (x == 1 && y == 0) {
			sprite.setRotation(0);
		}
		else if (x == 1 && y == 1) {
			sprite.setRotation(45);
		}
		else if (x == 0 && y == 1) {
			sprite.setRotation(90);
		}
		else if (x == -1 && y == 1) {
			sprite.setRotation(135);
		}
		else if (x == -1 && y == 0) {
			sprite.setRotation(180);
		}
		else if (x == -1 && y == -1) {
			sprite.setRotation(225);
		}
		else if (x == 0 && y == -1) {
			sprite.setRotation(270);
		}
		else if (x == 1 && y == -1) {
			sprite.setRotation(315);
		}
	}
};

class EnemySoldier: public Trooper{

private:
	sf::Vector2f movement;
	sf::Vector2f direction;
	sf::Texture enemySoldierTexture;
	sf::Vector2f position;
	sf::RenderWindow* window;
	float speed;
	int distance;
	random_device rd;
	int cooldown;

public :
	EnemySoldier(sf::RenderWindow*window) { //constructor
		this->window = window;
		

		enemySoldierTexture.loadFromFile("enemySoldier.png");
		sprite.setTexture(enemySoldierTexture);
		sprite.setScale(0.5f, 0.5f);
		sprite.setOrigin(98, 121);
		speed = .2f;
		int x = rd() % 1930;
		int y = rd() % 1030;
		sprite.setPosition(x, y);
		distance = 300;
		direction.x = 0;
		direction.y = 0;
		movement.x = 0;
		movement.y = 0;
		cooldown = 2;
	}
	void move(sf::Vector2f position) {
		float ePositionX = sprite.getPosition().x; //position of x of enemy
		movement.x = 0.f;
		movement.y = 0.f;
		if (position.x - ePositionX > distance || ePositionX - position.x > distance) {
			if (position.x < ePositionX) {
				movement.x = -speed;
			}
			else if (position.x > ePositionX) {
				movement.x = speed;
			}
		}
		//position.y;
		float ePositionY = sprite.getPosition().y;

		if (position.y - ePositionY > distance || ePositionY - position.y > distance) {
			if (position.y < ePositionY) {
				movement.y = -speed;
			}
			else if (position.y > ePositionY) {
				movement.y = speed;
			}
		}

		//cout << "movement: " << movement.x << "," << movement.y << endl;
		sprite.move(movement);
		changeDirection(position);


		for (int i = 0; i < bullets.size(); i++) {
			bullets[i]->move();
		}
		
	}
		void shoot(sf::Sound& sound) {
			if (cooldown == 0) {
				Bullet* bullet = new Bullet(sf::Color::Yellow);
				bullet->setDirection(direction);
				bullet->setPosition(sprite.getPosition());
				bullets.push_back(bullet);
				cooldown = 200;
				sound.play();
			}
			cooldown--;
		}
		
		sf::Sprite getEnemySoldierSprite() {
			return sprite;
		}
		void draw() {
			window->draw(sprite);
			for (int i = 0; i < bullets.size(); i++) {
				window->draw(bullets[i]->getCircleShape());
			}
		}

		void changeDirection(sf::Vector2f position) {//player's position
			float x = position.x-sprite.getPosition().x;
			float y = position.y-sprite.getPosition().y;
			int parameter = 100;

			if (x > 0 && (y <= parameter && y >= -parameter )) {
				sprite.setRotation(0);

				this->direction.x = 1;
				this->direction.y = 0;
			}
			else if ((x <= parameter && x >= parameter) && y > 0) {
				sprite.setRotation(90);

				this->direction.x = 0;
				this->direction.y = 1;
			}
			else if (x < 0 && (y <= parameter && y >= -parameter)) {
				sprite.setRotation(180);

				this->direction.x = -1;
				this->direction.y = 0;
			}
			else if ((x <= parameter && x >= -parameter) && y < 0) {
				sprite.setRotation(270);

				this->direction.x = 0;
				this->direction.y = -1;
			}
			else if (x > 0 && y > 0) {
				sprite.setRotation(45);

				this->direction.x = 1;
				this->direction.y = 1;
			}
			
			else if (x < 0 && y > 0) {
				sprite.setRotation(135);

				this->direction.x = -1;
				this->direction.y = 1;
			}
			
			else if (x < 0 && y < 0) {
				sprite.setRotation(225);

				this->direction.x = -1;
				this->direction.y = -1;
			}
			
			else if (x > 0 && y < 0) {
				sprite.setRotation(315);

				this->direction.x = 1;
				this->direction.y = -1;
			}
		}

		bool isNear(sf::Vector2f position) {
			float ePositionX = sprite.getPosition().x; //position of x of enemySoldier
			float ePositionY = sprite.getPosition().y;

			if (position.x - ePositionX > distance || ePositionX - position.x > distance || position.y - ePositionY > distance || ePositionY - position.y > distance) {
				return true;
			}

			return false;
		}
};



