// Author: Dr Noel O'Hara
// SFML Starter for an awesome game you can make!
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <SFML/Graphics.hpp>
// I use a couple of h files from thor library.
//https://github.com/Bromeon/Thor
#include "VectorAlgebra2D.h"


#include <iostream>
#include <stdlib.h> 
#include <time.h> 
#include <list>

using namespace std;

enum PlayerState { ready, drawingLine, Jump };
class Game
{
public:
	//create Window
	sf::RenderWindow window;
	sf::View view;
	float randomNum = 0;

	sf::CircleShape playerShape;
	sf::RectangleShape pillar[6];
	sf::Vector2f moveForce;
	PlayerState playerState = ready;
	float gravityScale = 20;
	sf::Vector2f velocity = { 0,0 };
	float playerMaxSpeed = 400;
	sf::Vector2f gravity{ 0.0f, 9.8f * gravityScale };
	
	sf::Vector2f normalised ;
	

	Game()
	{
		window.create(sf::VideoMode(800, 600), "Untitled Physics Games");
	}
	
	void init()
	{

		view = window.getDefaultView();
		playerShape.setRadius(20);
		playerShape.setPosition(160, 500);
		playerShape.setOrigin(sf::Vector2f(20, 20));
		sf::Vector2f normalised = thor::unitVector(playerShape.getPosition());
		float l = thor::length(gravity);
		float r = thor::toDegree(10.0);
		
		thor::setLength(normalised, 100.0f);
		thor::rotate(normalised, 10.0f);

		pillar[0].setSize(sf::Vector2f(10.0f, 360.0f));
		pillar[0].setOrigin(sf::Vector2f(0, 140.0f));
		pillar[0].setPosition(320, 500);

		pillar[1].setSize(sf::Vector2f(10.0f, 640.0f));
		pillar[1].setOrigin(sf::Vector2f(0, 60.0f));
		pillar[1].setPosition(370, 500);

		pillar[2].setSize(sf::Vector2f(10.0f, 450.0f));
		pillar[2].setOrigin(sf::Vector2f(0, 150.0f));
		pillar[2].setPosition(420, 500);

		pillar[3].setSize(sf::Vector2f(10.0f, 200.0f));
		pillar[3].setOrigin(sf::Vector2f(0, 400.0f));
		pillar[3].setPosition(470, 500);

		pillar[4].setSize(sf::Vector2f(10.0f, 180.0f));
		pillar[4].setOrigin(sf::Vector2f(0, 420.0f));
		pillar[4].setPosition(520, 500);
		
	}
	void run()
	{
	
		sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);


		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		
		sf::Clock clock;

		clock.restart();

		while (window.isOpen())
		{
			
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			
			timeSinceLastUpdate += clock.restart();

			

			if (timeSinceLastUpdate > timePerFrame)
			{

				if (playerState == ready)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						playerState = drawingLine;
					}
				}
				if (playerState == drawingLine)
				{
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						sf::Vector2f localPosition = (sf::Vector2f)sf::Mouse::getPosition(window);
						sf::Vector2f newVelocity;
						newVelocity = localPosition - playerShape.getPosition();

						if (thor::length(moveForce) < playerMaxSpeed)
						{
							velocity = newVelocity;
						}
						else {
							thor::setLength(moveForce, playerMaxSpeed);
							velocity = newVelocity;
						}

						playerState = Jump;
						gravity.y = 9.8 * gravityScale;
					}
				}
				if (playerState == Jump)
				{					
					playerState = ready;					
				}

				
				velocity = velocity + (gravity * timeSinceLastUpdate.asSeconds());
				
				playerShape.move(velocity.x * timeSinceLastUpdate.asSeconds(), velocity.y * timeSinceLastUpdate.asSeconds());



				if (playerShape.getPosition().y > 600 - playerShape.getRadius())
				{
					gravity.y = 0;
					velocity = { 0,0 };
					
				}
				
				window.clear();
				if (playerState == drawingLine)
				{
					sf::Vector2i localPosition = sf::Mouse::getPosition(window);
					sf::Vertex line[] =
					{
						sf::Vertex(sf::Vector2f(localPosition.x, localPosition.y)),
						sf::Vertex(sf::Vector2f(playerShape.getPosition().x, playerShape.getPosition().y))
					};

					window.draw(line, 2, sf::Lines);
				}
				window.draw(playerShape);				
				for (int i = 0; i < 6; i++)
				{
					window.draw(pillar[i]);
				}

				window.display();

				for (int i = 0; i < 6; i++)
				{
					if (playerShape.getGlobalBounds().intersects(pillar[i].getGlobalBounds()))
					{
						cout << i << " - colliding"<< endl;
						pillar[i].setFillColor(sf::Color::Red);
					}
					else
					{
						pillar[i].setFillColor(sf::Color::White);
					}
				}			
				timeSinceLastUpdate = sf::Time::Zero;
			}
		}
	}
};


int main()
{


	Game game;
	

	game.init();

	game.run();



	return 0;
}