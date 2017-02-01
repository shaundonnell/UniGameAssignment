#include "SceneManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "HealthPickup.h"


SceneManager::SceneManager(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label, POINT2f const & position)
	: GameObject(width, height, ui, scene, label, position), enemyList(), wallList(), pickupList()
{
	level = 1;
	gameOver = false;
	playerWon = false;

	player = Instantiate<Player>(20, 20);
	player->SetPosition(POINT2f(200, 200));

	LoadLevel();

}


SceneManager::~SceneManager()
{

}

void SceneManager::LoadLevel()
{

	std::ifstream file("Level0" + std::to_string(level) + ".txt");
	std::string str;
	int lineNumber = 2; // Start 2 lines down so the game UI can appear at the top
	int tileSize = 20;
	while(std::getline(file, str))
	{
		for(int i = 0; i < str.length(); i++)
		{
			char c = str[i];
			if(c == 'W')
			{
				auto wall = Instantiate<Wall>(tileSize, tileSize);
				wall->SetPosition(POINT2f(tileSize * i, tileSize * lineNumber));
				wallList.push_back(wall);
			}
			else if(c == 'E')
			{
				auto enemy = Instantiate<Enemy>(20, 20);
				enemy->SetPosition(POINT2f(tileSize * i, tileSize * lineNumber));
				enemy->SetFireTarget(player);
				enemyList.push_back(enemy);
			}
			else if(c == 'P')
			{
				player->SetPosition(POINT2f(tileSize * i, tileSize * lineNumber));
			}
			else if (c == 'H')
			{
				auto healthPickup = Instantiate<HealthPickup>(tileSize, tileSize);
				healthPickup->SetPosition(POINT2f(tileSize * i, tileSize * lineNumber));
				pickupList.push_back(healthPickup);
			}
		}
		lineNumber++;
	}
}

void SceneManager::input_impl()
{
}

void SceneManager::draw_impl(Canvas & c)
{
	if (gameOver)
	{
		TEXTURE_PTR youTexture = std::make_shared<WinTexture>("resource//You.bmp", RGB(255, 255, 255));
		TEXTURE_PTR stateTexture;

		// Get the win or lose texture based on the game state
		if(playerWon)
			stateTexture = std::make_shared<WinTexture>("resource//Win.bmp", RGB(255, 255, 255));
		else
			stateTexture = std::make_shared<WinTexture>("resource//Lose.bmp", RGB(255, 255, 255));

		// Place the text in the centre of the screen
		int halfWidth = scene->GetWidth() / 2;
		int halfHeight = scene->GetHeight() / 2;
		c.DrawTexture(*youTexture, POINT2f(halfWidth - 48 - 8, halfHeight - 8));
		c.DrawTexture(*stateTexture, POINT2f(halfWidth + 8, halfHeight - 8));

		return; // Don't need to process anything else if the game has ended
	}

	// Display a heart sprite for each health the player has
	TEXTURE_PTR heart = std::make_shared<WinTexture>("resource//heart.bmp", RGB(255, 255, 255));
	long heartColor = RGB(255, 0, 0);
	for (int i = 0; i < player->GetHealth(); i++)
	{
		int ulx = 5 + (20 * i);
		int uly = 5;

		c.DrawTexture(*heart, POINT2f(ulx, uly));
	}

	// Display the Level sprites
	if (level <= 5)
	{
		TEXTURE_PTR levelTexture = std::make_shared<WinTexture>("resource//Level.bmp", RGB(255, 255, 255));
		std::string levelNumberBmp = "resource//" + std::to_string(level) + ".bmp";
		TEXTURE_PTR levelNumberTexture = std::make_shared<WinTexture>(levelNumberBmp, RGB(255, 255, 255));

		c.DrawTexture(*levelTexture, POINT2f(scene->GetWidth() - 5 - 96 - 8, 5));
		c.DrawTexture(*levelNumberTexture, POINT2f(scene->GetWidth() - 5 - 16, 5));
	}
}

void SceneManager::update_impl(float const & dt)
{
	if (gameOver)
		return; // Don't need to process anything else if the game has ended

	if (player->GetHealth() <= 0)
	{
		GameOver();
	}
	else
	{
		if (enemyList.size() == 0)
		{
			ClearLevel();

			if (level >= 6)
			{
				YouWin();
			}
			else
			{
				level++;
				LoadLevel();
			}
		}
	}
}

void SceneManager::EntityDestroyed(GAMEOBJECT_PTR entity)
{
	ENEMY_PTR enemy = std::dynamic_pointer_cast<Enemy>(entity);
	if(enemy)
	{
		printf("removing enemy");
		enemyList.remove(enemy);
	}

}

void SceneManager::GameOver()
{
	gameOver = true;
	playerWon = false;
	ClearLevel();
	player->Destroy();
}

void SceneManager::YouWin()
{
	gameOver = true;
	playerWon = true;
	ClearLevel();
	player->Destroy();
}

void SceneManager::ClearLevel()
{
	for (auto wall : wallList)
	{
		wall->Destroy();
	}
	wallList.clear();

	for (auto enemy : enemyList)
	{
		enemy->Destroy();
	}
	enemyList.clear();

	for (auto pickup : pickupList)
	{
		pickup->Destroy();
	}
	pickupList.clear();
}


