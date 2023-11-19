#include "InTheArmyNow.h"
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>


using namespace std;

int main() {

	const int Width = 1920, Height = 1080; //Screen Size
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Hold the line.....");

	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("rifle1.wav"))
		return -1;
	sf::Sound playerRifle;
	playerRifle.setVolume(45);
	playerRifle.setBuffer(buffer);
	
	sf::SoundBuffer buffer1;

	sf::Sound enemyRifle;
	if (!buffer1.loadFromFile("semiautorifle2.wav"))
		cout << "Couldn't load sound" << endl;

	enemyRifle.setVolume(45);
	enemyRifle.setBuffer(buffer1);
	
	sf::Texture backText; //Background
	backText.loadFromFile("Warzone.png");
	sf::Sprite backSprite;
	backSprite.setTexture(backText);

	PlayerSoldier*   soldier = new PlayerSoldier(&window);
	EnemySoldier*   enemy = new EnemySoldier(&window);

	sf::Text myText;
	sf::Font myFont;
	myFont.loadFromFile("they-perished/TheyPerished.ttf");
	myText.setFont(myFont);
	myText.setString("Your position is being overrun. Surrounded by the enemy, you only have one objective now soldier: SURVIVE" );
	myText.setCharacterSize(16);
	myText.setFillColor(sf::Color::White);
	myText.setPosition(560, 140);

	sf::Text deathText;
	sf::Font deathFont;
	deathFont.loadFromFile("they-perished/TheyPerished.ttf");
	deathText.setFont(deathFont);
	deathText.setString("You have died on the battlefield.......");
	deathText.setCharacterSize(24);
	deathText.setFillColor(sf::Color::White);
	deathText.setPosition(960, 540);


	bool MovingUp = false;
	bool MovingDown = false;
	bool MovingLeft = false;
	bool MovingRight = false;

	bool facingUp = false;
	bool facingDown = false;
	bool facingLeft = false;
	bool facingRight = false;

	bool Shooting = false;
	float firingDelay = 0.5;
	float firingCounter = 0;

	sf::Music music;

	vector<EnemySoldier*> enemySoldiers;

	bool soldierUp = false, soldierDown = false, soldierLeft = false, soldierRight = false; //Direction player is facing

	sf::Clock clock;
	sf::Time time;

	sf::RectangleShape blackScreen;
	blackScreen.setSize(sf::Vector2f(1920, 1080));
	blackScreen.setFillColor(sf::Color::Black);
	blackScreen.setPosition(0, 0);

	bool death = false;
	while (window.isOpen())
	{
		time = clock.getElapsedTime();
		//generating random enemies

		if (death == false) {
			if (time.asSeconds() >= 5) {
				EnemySoldier* enemySoldier = new EnemySoldier(&window);
				enemySoldiers.push_back(enemySoldier);
				clock.restart();
			}
		}
		if (music.getStatus() == sf::Music::Status::Stopped) { //Music
			random_device rd;
			int musicChoice = rd() % 4;
			cout << musicChoice << endl;
			switch (musicChoice) {
			case 0:
				music.openFromFile("ytmp3free.cc_battle-of-iwo-jima-youtubemp3free.org.ogg");
				music.setVolume(35);
				music.play();
				break;
			case 1:
				music.openFromFile("ytmp3free.cc_glorifica-youtubemp3free.org.ogg");
				music.setVolume(35);
				music.play();
				break;
			case 2:
				music.openFromFile("ytmp3free.cc_operation-whiteout-loading-screen-music-battlefield-4-youtubemp3free.org.ogg");
				music.setVolume(35);
				music.play();
				break;
			case 3:
				music.openFromFile("ytmp3free.cc_battlefield-3-operation-riverside-loading-screen-youtubemp3free.org.ogg");
				music.setVolume(35);
				music.play();
				break;
			default:
				music.stop();
			}
		}
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed) //Starts player movement
			{
				if (event.key.code == sf::Keyboard::W)
				{
					MovingUp = true;
				}
				if (event.key.code == sf::Keyboard::S)
				{
					MovingDown = true;
				}
				if (event.key.code == sf::Keyboard::A)
				{
					MovingLeft = true;
				}
				if (event.key.code == sf::Keyboard::D)
				{
					MovingRight = true;
				}
				if (event.key.code == sf::Keyboard::Up) //Player Direction
				{
					facingUp = true;
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					facingDown = true;
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					facingLeft = true;
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					facingRight = true;
				}
				//Start shooting
				if (event.key.code == sf::Keyboard::F) {
					soldier->shoot();
					playerRifle.play();
				}
			}

			if (event.type == sf::Event::KeyReleased) //To stop moving
			{
				if (event.key.code == sf::Keyboard::W)
				{
					MovingUp = false;
				}
				if (event.key.code == sf::Keyboard::S)
				{
					MovingDown = false;
				}
				if (event.key.code == sf::Keyboard::A)
				{
					MovingLeft = false;
				}
				if (event.key.code == sf::Keyboard::D)
				{
					MovingRight = false;
				}
				if (event.key.code == sf::Keyboard::Up) //Directions
				{
					facingUp = false;
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					facingDown = false;
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					facingLeft = false;
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					facingRight = false;
				}
				if (event.key.code == sf::Keyboard::F) { //Stop shooting

				}

			}
			
			
		}

		//checks if enemysoldiers died or player soldier died
		for (int i = 0; i < enemySoldiers.size(); i++) {
			if (soldier->checkHit(enemySoldiers[i])) {
				enemySoldiers.erase(enemySoldiers.begin()+i);
			}
		}
		for (int i = 0; i < enemySoldiers.size(); i++) {
			if (enemySoldiers[i]->checkHit(soldier)) {
				soldier->die();
				death = true;
				break;
			}
		}

		
		sf::Vector2f movement(0, 0);

		if (MovingUp)
			movement.y -= 0.5f;
		if (MovingDown)
			movement.y += 0.5f;
		if (MovingRight)
			movement.x += 0.5f;
		if (MovingLeft)
			movement.x -= 0.5f;

		sf::Vector2f direction(0, 0);
		if (facingUp)
			direction.y = -1.0f;
		if (facingDown)
			direction.y = 1.0f;
		if (facingRight)
			direction.x = 1.0f;
		if (facingLeft)
			direction.x = -1.0f;

		sf::Vector2f scroll(0, -.10f);
		myText.move(scroll);
		soldier->move(movement);
		soldier->changeDirection(direction);
		for (int i = 0; i < enemySoldiers.size(); i++) {
			enemySoldiers[i]->move(soldier->getSoldierSprite().getPosition());
		}

		//enemysoldier shoot
		for (int i = 0; i < enemySoldiers.size(); i++) {
			enemySoldiers[i]->shoot(enemyRifle);

		}
		window.clear();
		window.draw(backSprite);
		window.draw(myText);

		soldier->draw();
		for (int i = 0; i < enemySoldiers.size(); i++) {
			enemySoldiers[i]->draw();
		}

		soldier->updateBullets();
		for (int i = 0; i < enemySoldiers.size(); i++) {
			enemySoldiers[i]->updateBullets();
		}

		if (death == true) {
			window.draw(blackScreen);
			window.draw(deathText);
			music.stop();
		}
		
		window.display();


	}
	return 0;
}
