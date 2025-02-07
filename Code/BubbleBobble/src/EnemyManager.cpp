#include "EnemyManager.h"
#include "Slime.h"
#include "Turret.h"


EnemyManager::EnemyManager()
{
	shots = nullptr;
	map = nullptr;
}
EnemyManager::~EnemyManager()
{
	Release();
}
AppStatus EnemyManager::Initialise()
{
	return AppStatus::OK;
}
void EnemyManager::SetShotManager(ShotManager* shots)
{
	this->shots = shots;
}
void EnemyManager::SetTileMap(TileMap* level) {
	map = level;
}
void EnemyManager::Add(const Point& pos, EnemyType type, const AABB& area, Look look)
{


	Enemy* enemy = nullptr;

	if (type == EnemyType::SLIME)
	{
		enemy = new Slime(pos, SLIME_PHYSICAL_WIDTH, SLIME_PHYSICAL_HEIGHT, SLIME_FRAME_SIZE, SLIME_FRAME_SIZE, map);
	}
	else if (type == EnemyType::TURRET)
	{
		enemy = new Turret(pos, TURRET_PHYSICAL_WIDTH, TURRET_PHYSICAL_HEIGHT, TURRET_FRAME_SIZE, TURRET_FRAME_SIZE);
	}
	else if (type == EnemyType::DRUNK)
	{
		enemy = new Drunk(pos, SLIME_PHYSICAL_WIDTH, SLIME_PHYSICAL_HEIGHT, SLIME_FRAME_SIZE, SLIME_FRAME_SIZE, map);
	}
	else if (type == EnemyType::SD)
	{
		enemy = new SD(pos, SD_PHYSICAL_WIDTH, SD_PHYSICAL_HEIGHT, SLIME_FRAME_SIZE, SLIME_FRAME_SIZE, map);
	}
	else if (type == EnemyType::DSLIME)
	{
		enemy = new DSLIME(pos, SLIME_PHYSICAL_WIDTH, SLIME_PHYSICAL_HEIGHT, SLIME_FRAME_SIZE, SLIME_FRAME_SIZE, map);
	}
	else if (type == EnemyType::DDRUNK)
	{
		enemy = new DDRUNK(pos, SLIME_PHYSICAL_WIDTH, SLIME_PHYSICAL_HEIGHT, SLIME_FRAME_SIZE, SLIME_FRAME_SIZE, map);
	}

	else
	{
		LOG("Internal error: trying to add a new enemy with invalid type");
		return;
	}

	if (enemy)
	{
		enemy->Initialise(look, area);
		enemies.push_back(enemy);
	}
	else
	{
		LOG("Failed to create enemy");
	}
}

AABB EnemyManager::GetEnemyHitBox(const Point& pos, EnemyType type) const
{
	int width, height;
	if (type == EnemyType::SLIME)
	{
		width = SLIME_PHYSICAL_WIDTH;
		height = SLIME_PHYSICAL_HEIGHT;
	}
	else if (type == EnemyType::TURRET)
	{
		width = TURRET_PHYSICAL_WIDTH;
		height = TURRET_PHYSICAL_HEIGHT;
	}
	else if (type == EnemyType::SD)
	{
		width = SD_PHYSICAL_WIDTH;
		height = SD_PHYSICAL_HEIGHT;
	}
	else
	{
		LOG("Internal error while computing hitbox for an invalid enemy type");
		return {};
	}
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
void EnemyManager::Update(const AABB& player_hitbox)
{
	bool shoot;
	Point p, d;

	for (Enemy* enemy : enemies)
	{
		shoot = enemy->Update(player_hitbox);
		if (shoot)
		{
			enemy->GetShootingPosDir(&p, &d);
			shots->Add(p, d);
			LOG("Added shot at position: ", p.x, p.y);  

		}
	}
	
}
void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
		enemy->Draw();
}
void EnemyManager::DrawDebug() const
{
	Color c;
	c.r = 128;
	c.b = 128;
	c.g = 0;
	c.a = 50;

	for (const Enemy* enemy : enemies)
	{
		enemy->DrawHitbox(c);
	}
}
void EnemyManager::DestroyEnemy(Enemy* enemy)
{
	auto it = std::find(enemies.begin(), enemies.end(), enemy);
	if (it != enemies.end())
	{
		delete* it;
		enemies.erase(it);
	}
}
void EnemyManager::Release()
{
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();
}