#include "Core.hpp"

Core::Core() : mWindow(nullptr), mDelaTime(0.F), mIsFired(false), mIsSideways(false)
{
	mWindow = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Rotation");

	loadTextures();

	setAnimations();

	mTurretSprite.setPosition(sf::Vector2f(mSprite.getPosition().x + 130, mSprite.getPosition().y + 150));
}

Core::~Core() 
{ 
	delete mWindow; 

	delete mAnimationComponent;

	delete mMovementComponent;
}

void Core::run()
{
	while (mWindow->isOpen())
	{
		render(*mWindow);

		update(mDelaTime);

		dtClock();
	}
}

void Core::updateAnimations(float& deltaTime)
{
	if (mMovementComponent->getState(IDLE))
		mAnimationComponent->play("IDLE", deltaTime);

	else if (mMovementComponent->getState(MOVING_UP))
		mAnimationComponent->play("MOVE", deltaTime);

	else if (mMovementComponent->getState(MOVING_LEFT))
		mAnimationComponent->play("SIDE", deltaTime);
}

void Core::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	mMovementComponent = new MovementComponent(mTreadSprite, max_velocity, acceleration, deceleration);
}

void Core::createAnimationComponent(sf::Texture& texture_sheet)
{
	mAnimationComponent = new AnimationComponent(mTreadSprite, texture_sheet);
}

void Core::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (mMovementComponent)
		mMovementComponent->move(direction_x, direction_y, deltaTime);
}

void Core::setAnimations()
{
	createMovementComponent(350.F, 16.F, 6.F);

	createAnimationComponent(mTreadTexture);

	mAnimationComponent->addAnimation("IDLE", 50.F, 0, 0, 1, 0, 256, 256);
	mAnimationComponent->addAnimation("MOVE", 16.F, 0, 0, 1, 0, 256, 256);
	mAnimationComponent->addAnimation("SIDE", 16.F, 1, 0, 1, 1, 256, 256);
}

void Core::update(float& deltaTime)
{
	mMovementComponent->update(deltaTime);

	updateAnimations(deltaTime);

	processEvents(deltaTime);

	moveSprite(deltaTime);

	mPosition = mTurretSprite.getPosition();

	mTurretSprite.setPosition(sf::Vector2f(mSprite.getPosition().x + 130, mSprite.getPosition().y + 150));

	mTreadSprite.setPosition(sf::Vector2f(mSprite.getPosition().x, mSprite.getPosition().y - 15));
}

void Core::render(sf::RenderTarget & target)
{
	mWindow->clear();

	target.draw(mTreadSprite);

	target.draw(mSprite);

	target.draw(mTurretSprite);

	if (mIsFired)
		target.draw(mProjectileSprite);

	mWindow->display();
}

void Core::processEvents(float & deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		mSprite.move(0.F, -1.F);

		//move(0.F, -1.F, deltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		mSprite.move(0.F, 1.F);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		mSprite.move(-1.F, 0.F);

		//move(-1.F, 0.F, deltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		mSprite.move(1.F, 0.F);

		//move(-1.F, 0.F, deltaTime);
	}

	while (mWindow->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			mWindow->close();

		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
			mWindow->close();

		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			mIsFired = true;*/
	}
}

void Core::dtClock() { mDelaTime = mDTClock.restart().asSeconds(); }

void Core::moveSprite(float & deltaTime)
{
	position = mTurretSprite.getPosition();
	curPos = sf::Mouse::getPosition(*mWindow);
	sf::Vector2f moveDirection;
	sf::Vector2f bulletSpawnPosition = mTurretSprite.getPosition();


	float dx = curPos.x - position.x;
	float dy = curPos.y - position.y;

	float rotation = (atan2(dy, dx) * 180) / thor::Pi;

	mTurretSprite.setRotation(rotation + 90.F);

	//mProjectileSprite.setPosition(mPosition.x + 100, mPosition.y);

	moveDirection.x = curPos.x - bulletSpawnPosition.x;
	moveDirection.y = curPos.y - bulletSpawnPosition.y;

	sf::Vector2f normDirection;

	normDirection = normalize(moveDirection, vectorLength(moveDirection));
	mProjectileSprite.move(normDirection.x * deltaTime, normDirection.y * deltaTime);
}

void Core::loadTextures()
{
	if (!mTexture.loadFromFile("Hull.png"))
		std::cerr << "";

	if (!mTurretTexture.loadFromFile("Gun.png"))
		std::cerr << "";

	if (!mProjectileTexture.loadFromFile("Shell.png"))
		std::cerr << "";

	if (!mTreadTexture.loadFromFile("C.png"))
		std::cerr << "";

	mProjectileSprite.setTexture(mProjectileTexture);

	mSprite.setTexture(mTexture);

	mTurretSprite.setTexture(mTurretTexture);

	mTreadSprite.setTexture(mTreadTexture);

	mSprite.setPosition(sf::Vector2f(800.F, 400.F));

	mTurretSprite.setOrigin(mTurretSprite.getLocalBounds().width / 2, mTurretSprite.getLocalBounds().height - 50.F);
}

void Core::checkDirection(float& deltaTime)
{
}

float Core::vectorLength(sf::Vector2f v)
{
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

sf::Vector2f Core::normalize(sf::Vector2f v, float length)
{
	if (length == 0)
		return sf::Vector2f(0.f, 0.f);
	else
		return v / length;
}
