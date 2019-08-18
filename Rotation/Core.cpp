#include "Core.hpp"

Core::Core() : mWindow(nullptr), mDelaTime(0.F), mIsFired(false)
{
	mWindow = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Rotation");

	loadTextures();

	//setAnimations();

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

void Core::updateAnimations(float & deltaTime)
{
	if (mMovementComponent->getState(MOVING))
	{
		if (mSprite.getScale().y < 0.f)
		{
			mSprite.setOrigin(0.f, 0.f);
			mSprite.setScale(1.f, 1.f);
		}

		this->mAnimationComponent->play("MOVE", deltaTime, mMovementComponent->getVelocity().y, mMovementComponent->getMaxVelocity());
	}

	else if (this->mMovementComponent->getState(MOVING))
	{
		if (mSprite.getScale().y < 0.f)
		{
			mSprite.setOrigin(140.f, 0.f);
			mSprite.setScale(1.f, 1.f);
		}

		mAnimationComponent->play("MOVE", deltaTime, mMovementComponent->getVelocity().y, mMovementComponent->getMaxVelocity());
	}
}

void Core::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	mMovementComponent = new MovementComponent(mSprite, max_velocity, acceleration, deceleration);
}

void Core::createAnimationComponent(sf::Texture& texture_sheet)
{
	mAnimationComponent = new AnimationComponent(mSprite, texture_sheet);
}

void Core::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (mMovementComponent)
		mMovementComponent->move(direction_x, direction_y, deltaTime);
}

void Core::setAnimations()
{
	createMovementComponent(350.F, 16.F, 6.F);

	createAnimationComponent(mTexture);

	mAnimationComponent->addAnimation("MOVE", 16.F, 0, 0, 4, 0, 256, 256);
}

void Core::update(float& deltaTime)
{
	processEvents(deltaTime);

	moveSprite(deltaTime);

	mPosition = mTurretSprite.getPosition();

	mTurretSprite.setPosition(sf::Vector2f(mSprite.getPosition().x + 130, mSprite.getPosition().y + 150));

	//updateAnimations(deltaTime);
}

void Core::render(sf::RenderTarget & target)
{
	mWindow->clear();

	target.draw(mSprite);

	target.draw(mTurretSprite);

	if (mIsFired)
		target.draw(mProjectileSprite);

	mWindow->display();
}

void Core::processEvents(float & deltaTime)
{
	while (mWindow->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			mWindow->close();

		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
			mWindow->close();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			mSprite.move(0.F, -1.F);

			move(0.F, -1.F, deltaTime);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			mSprite.move(0.F, 1.F);

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

	if (!mTreadTexture_L.loadFromFile("Track_1_A.png"))
		std::cerr << "";

	if (!mTreadTexture_R.loadFromFile("Track_1_B.png.png"))
		std::cerr << "";

	mProjectileSprite.setTexture(mProjectileTexture);
	mSprite.setTexture(mTexture);
	mTurretSprite.setTexture(mTurretTexture);

	mSprite.setPosition(sf::Vector2f(800.F, 400.F));

	mTurretSprite.setOrigin(mTurretSprite.getLocalBounds().width / 2, mTurretSprite.getLocalBounds().height - 50.F);
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
