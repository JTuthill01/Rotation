#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Math.hpp>
#include <iostream>
#include <glm/glm.hpp>

#include "AnimationComponent.h"
#include "MovementComponent.h"

class Core
{
public:
	Core();
	~Core();

	void run();

private:
	void updateAnimations(float& deltaTime);
	void createMovementComponent(const float max_velocity, const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);
	void move(const float direction_x, const float direction_y, const float& deltaTime);
	void setAnimations();
	void update(float& deltaTime);
	void render(sf::RenderTarget& target);
	void processEvents(float& deltaTime);
	void dtClock();
	void moveSprite(float& deltaTime);
	void loadTextures();
	void checkDirection(float& deltaTime);

	float vectorLength(sf::Vector2f v);
	sf::Vector2f normalize(sf::Vector2f v, float length);

	sf::Vector2f mPosition;
	sf::RenderWindow* mWindow;
	sf::Event e;
	sf::Clock mDTClock;

	sf::Sprite mProjectileSprite;
	sf::Texture mProjectileTexture;
	
	sf::Sprite mSprite;
	sf::Texture mTexture;

	sf::Sprite mTurretSprite;
	sf::Texture mTurretTexture;

	sf::Sprite mTreadSprite;
	sf::Texture mTreadTexture;

	float mDelaTime;

	bool mIsFired;
	bool mIsSideways;

	AnimationComponent* mAnimationComponent;
	MovementComponent* mMovementComponent;

	sf::Vector2f position;
	sf::Vector2i curPos;
};

