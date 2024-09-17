#include "Entity.h"

#include "GameEngine.h"
#include "Debug.h"
#include "Random.h"
#include "Misc Functions.h"
#include "Vector2D.h"

Bullets allBullets = Bullets();
Asteroids allAsteroids = Asteroids();

// Constructor.
Entity::Entity()
{
	tex = nullptr;
	x = y = 0.0f;
	rotation = 0.0f;
}

// Base draw for entities.
// draws the entity to its stored position.
void Entity::Draw()
{
	if (tex != nullptr)
	{
		tex->Draw(game.GetRenderer().renderer, rotation, static_cast <int> (round(x)), static_cast <int> (round(y)));
	}
}

// Constructor.
Player::Player()
{
	// Set default values.
	velX = velY = 0.0f;
	velocity = 0.0f;
	turnRate = 0.0f;
	speedBoost = false;
	fireTimer = 0;
	fireInterval = 0;
	immunityTime = 0;

	moveForwardsSound = nullptr;
	moveForwardsBoostSound = nullptr;
	moveBackwardsSound = nullptr;
	moveBackwardsBoostSound = nullptr;
	equalizeSound = nullptr;
}

// Update players position and other data.
bool Player::Update()
{
	// Make sure player hasnt gone over max velocity.
	// Checking positive velocity.
	if (velX > game.MAX_VEL)
	{
		velX = game.MAX_VEL;
	} 
	else if (velX < -game.MAX_VEL)
	{
		velX = -game.MAX_VEL;
	}
	if (velY > game.MAX_VEL)
	{
		velY = game.MAX_VEL;
	}	
	else if (velY < -game.MAX_VEL)
	{
		velY = -game.MAX_VEL;
	}

	// Add the velocity to its respective axis.
	x += velX;
	y += velY;

	// Check if player has gone off screen and mvoe them to the other side.
	if (x < 0)
	{
		x = static_cast <float> (game.SCREEN_WIDTH - 1);
	}
	else if (x > game.SCREEN_WIDTH)
	{
		x = 0;
	}

	if (y < 0)
	{
		y = static_cast <float> (game.SCREEN_HEIGHT - 1);
	}
	else if (y > game.SCREEN_HEIGHT)
	{
		y = 0;
	}

	// Reset the speed boost back to false, otherwise the player will permanently be boosted.
	speedBoost = false;

	if (fireTimer > 0)
	{
		fireTimer--;
	}

	if (immunityTime > 0)
	{
		immunityTime--;
	}

	return true;
}

// Fire players weapon, creating a bullet at location.
void Player::FireWeapon()
{
	if (fireTimer <= 0)
	{
		fireTimer = fireInterval;
		allBullets.CreateBullet(this, game.State()->allTextures.GetTexture("bullet"));
		game.State()->allSounds.GetSound("shot")->Play();
	}
}

// Respawn the player in middle of screen.
void Player::Respawn()
{
	velX = velY = 0;
	x = static_cast<float> (game.SCREEN_WIDTH) / 2.0f;
	y = static_cast<float> (game.SCREEN_HEIGHT) / 2.0f;
	immunityTime = 180;
}

// Constructor.
Bullet::Bullet()
{
	distanceLeft = 0;
	velX = velY = 0.0f;
	speed = 0.0f;
	startX = startY = 0.0f;
}
// Update the position of the bullet.
bool Bullet::Update()
{
	// Update position,
	x -= velX;
	y -= velY;
	distanceLeft--;

	// Check if bullet has goen off screen and warp it if need be.
	if (x < 0)
	{
		x = static_cast <float> (game.SCREEN_WIDTH - 1) + x;
	}
	else if (x > game.SCREEN_WIDTH)
	{
		x = 0 + (x - game.SCREEN_WIDTH);
	}
	if (y < 0)
	{
		y = static_cast <float> (game.SCREEN_HEIGHT - 1) + y;
	}
	else if (y > game.SCREEN_HEIGHT)
	{
		y = 0 + (y - game.SCREEN_HEIGHT);
	}

	// Check if bullet has hit end life and delete it.
	if (distanceLeft < 0)
	{
		return false;
	}

	return true;
}

// Remove the bullet from the list of all bullets and delete it.
void Bullet::Destroy()
{
	allBullets.DestroyBullet(this);
}

// Create a bullet and add it to the list of all bullets.
void Bullets::CreateBullet(Player* player, Texture* tex)
{
	Bullet* bullet = new Bullet();

	bullet->x = player->x;
	bullet->y = player->y;
	bullet->startX = bullet->x;
	bullet->startY = bullet->y;
	bullet->velX = static_cast <float> ( cos( (player->rotation + 90.0f) * (M_PI / 180) ) ) * ((player->velX * 0.25f) + game.BULLET_VELOCITY);
	bullet->velY = static_cast <float> ( sin( (player->rotation + 90.0f) * (M_PI / 180) ) ) * ((player->velY * 0.25f) + game.BULLET_VELOCITY);
	bullet->distanceLeft = game.BULLET_DISTANCE;
	bullet->tex = tex;	

	allBullets.push_back(bullet);

	debug.Log("Bullets", "CreateBullet", "Bullet was created at " + std::to_string(bullet->x) + "\\" + std::to_string(bullet->y) + " going to " + std::to_string(bullet->velX) + "\\" + std::to_string(bullet->velY));
}

// Remove the given bullet from the list of bullets then delete it
void Bullets::DestroyBullet(Bullet* bullet)
{
	allBullets.remove(bullet);
	delete bullet;
	bullet = nullptr;

	debug.Log("Bullets", "DestroyBullet", "Removed and destroyed bullet from list");
}

// Iterate over the list of bullets and update them all.
void Bullets::UpdateAll()
{
	for (auto it = allBullets.begin(); it != allBullets.end();)
	{
		if (!(*it)->Update())
		{
			// Delete the dereferenced iterator.
			delete (*it);
			// Update the list and get the next location.
			it = allBullets.erase(it);
			debug.Log("Bullets", "UpdateAll", "Bullet ran out of distance and was deleted.");
		}
		else
		{
			it++;
		}
	}
}

// Iterate over all bullets and render each one in order.
void Bullets::RenderAll()
{
	for (auto& var : allBullets)
	{
		var->Draw();
	}
}

// Iterate over all bullets and delete them
void Bullets::Clear()
{
	for (auto bullet : allBullets)
	{
		delete bullet;
	}
	
	allBullets.clear();

	debug.Log("Bullets", "Clear", "Removed and destroyed ALL bullets from list");
}

// Constructor.
Asteroid::Asteroid()
{
	velX = velY = 0.0f;
	size = 0;
}

// Update position of all asteroids.
bool Asteroid::Update()
{
	x -= velX;
	y -= velY;

	if (x < 0)
	{
		x = static_cast <float> (game.SCREEN_WIDTH - 1);
	}
	else if (x > game.SCREEN_WIDTH)
	{
		x = 0;
	}

	if (y < 0)
	{
		y = static_cast <float> (game.SCREEN_HEIGHT - 1);
	}
	else if (y > game.SCREEN_HEIGHT)
	{
		y = 0;
	}

	return true;
}

// Break the current asteroid.
void Asteroid::Break(Bullet* bullet)
{
	// If smallest size then destoy the asteroid
	if (size == 10)
	{
		allAsteroids.DestroyAsteroid(this);
	}
	else
	{
		Random random;
		
		// Split the meteor in two.
		int splits = 2;
		while (splits > 0)
		{
			float calcualtedVelX;
			float calcualtedVelY;

			// Check if we have a bullet or not.
			// If we do calculate using its trajectory.
			// Otherwise just randomise the split direction.
			if (bullet != nullptr)
			{
				// Use a mathimatical 2D vector to determine what velocity the asteroid will be moving at.
				Vector2D diffVec(x - bullet->startX, y - bullet->startY);
				diffVec.Normalize();
				diffVec.Multiply(game.MAX_ASTEROID_VEL);

				// Set the velocity for the new meteor.
				calcualtedVelX = -(diffVec.x * (random.RandomFloat(0, 1)));
				calcualtedVelY = -(diffVec.y * (random.RandomFloat(0, 1)));
			}
			else
			{
				// Set the velocity for the new meteor.
				calcualtedVelX = random.RandomFloat(-game.MAX_ASTEROID_VEL, game.MAX_ASTEROID_VEL);
				calcualtedVelY = random.RandomFloat(-game.MAX_ASTEROID_VEL, game.MAX_ASTEROID_VEL);
			}

			allAsteroids.CreateAsteroid(static_cast <int> (round(x)), static_cast <int> (round(y)), calcualtedVelX, calcualtedVelY, size - 5);
			splits--;
		}

		// Destroy the original asteroid.
		allAsteroids.DestroyAsteroid(this);
	}	
}

// Create an asteroid at a specific location and add it the the asteroids list.
void Asteroids::CreateAsteroid(int x, int y, float velX, float velY, int size)
{
	Asteroid* asteroid = new Asteroid();

	asteroid->velX = velX;
	asteroid->velY = velY;

	asteroid->size = size;
	asteroid->tex = game.State()->allTextures.GetTexture("asteroid " + std::to_string(asteroid->size));

	asteroid->y = static_cast <float> (y);
	asteroid->x = static_cast <float> (x);

	allAsteroids.push_back(asteroid);

	debug.Log("Asteroids", "CreateAsteroid", "Created asteroid at " + std::to_string(asteroid->x) + "\\" + std::to_string(asteroid->y) + " with velocity " + std::to_string(asteroid->velX) + "\\" + std::to_string(asteroid->velY) + " at size of " + std::to_string(asteroid->size));
}

// Randomly create an asteroid and spawn it at the edges of the map.
void Asteroids::CreateAsteroid(Player* player)
{
	int randomNum;

	Random random;

	bool finished = false;
	int x;
	int y;
	float velX;
	float velY;
	int size;

	while (!finished)
	{
		x = 0;
		y = 0;
		velX = random.RandomFloat(-game.MAX_ASTEROID_VEL, game.MAX_ASTEROID_VEL);
		velY = random.RandomFloat(-game.MAX_ASTEROID_VEL, game.MAX_ASTEROID_VEL);

		// Setup the asteroids size.
		// Asteroid sizes are in multiples of 5.
		size = 5 * (random.RandomInt(2, 5));

		randomNum = random.RandomInt(0, 4);
		switch (randomNum)
		{
		case 0: // spawn randomly at left of screen.
			y = random.RandomInt(0, game.SCREEN_HEIGHT);
			break;

		case 1:// spawn randomly at right of screen.
			y = random.RandomInt(0, game.SCREEN_HEIGHT);
			x = game.SCREEN_WIDTH - 1;
			break;

		case 2:// spawn randomly at top of screen;
			x = random.RandomInt(0, game.SCREEN_WIDTH);
			break;

		case 3:// spawn randomly at bottom of screen;
			x = random.RandomInt(0, game.SCREEN_WIDTH);
			y = game.SCREEN_HEIGHT - 1;
			break;
		}

		float distance = GetDistance(static_cast <float> (x), static_cast <float> (y), player->x, player->y);

		if (distance >= game.AUTO_SPAWN_ASTEROIDS_DISTANCE_FROM_PLAYER)
			finished = true;
	}	

	CreateAsteroid(x, y, velX, velY, size);
}

// Remove an asteroid from the list and delete it.
void Asteroids::DestroyAsteroid(Asteroid* asteroid)
{
	allAsteroids.remove(asteroid);
	delete asteroid;

	debug.Log("Asteroids", "DestroyAsteroid", "Removed and destroyed asteroid from list");
}

// Update the position of all asteroids in the list.
void Asteroids::UpdateAll()
{
	for (auto& var : allAsteroids)
		var->Update();
}

// Render all asteroids in the list.
void Asteroids::RenderAll()
{
	for (auto& var : allAsteroids)
		var->Draw();
}

// Remove all asteroids from the list and delete them.
// Mainly used for clearing data on shutdown.
void Asteroids::Clear()
{
	for (auto asteroid : allAsteroids)
		delete asteroid;

	allAsteroids.clear();

	debug.Log("Asteroids", "Clear", "Removed and destroyed ALL asteroids from list");
}