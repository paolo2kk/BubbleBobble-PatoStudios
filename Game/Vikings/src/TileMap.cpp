#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	laser = nullptr;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	if (laser != nullptr)
	{
		laser->Release();
		delete laser;
		laser = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::BLOCKWITH1] = { 0, n, n, n };
	dict_rect[(int)Tile::BLOCKWITHOUT1] = { 0, 2 * n, n, n };
	dict_rect[(int)Tile::CORNER] = { n, n, n, n };

	dict_rect[(int)Tile::PLATFORMBASIC] = { 2 * n, n, n, n };
	dict_rect[(int)Tile::PLATFORMBEGINNING] = { 3* n, 6 * n, n, n };
	dict_rect[(int)Tile::PLATFORMEND] = { 12 * n, 6 * n, n, n };
	dict_rect[(int)Tile::PLATFORMCORNERRIGHT] = { 14 * n, 6 * n, n, n };
	dict_rect[(int)Tile::PLATFORMMIDDLESTART] = { 3 * n, 8 * n, n, n };
	dict_rect[(int)Tile::PLATFORMMIDDLEFINISH] = { 12 * n, 8 * n, n, n };

	dict_rect[(int)Tile::SHADOW] = { 2 * n, 6 * n, n, n };
	dict_rect[(int)Tile::SHADOW2] = { n, 3 * n, n, n };
	dict_rect[(int)Tile::SHADOW3] = { 2 * n, 9 * n, n, n };
	dict_rect[(int)Tile::SHADOW4] = { 14 * n, 9 * n, n, n };
	dict_rect[(int)Tile::SHADOW5] = { 12 * n, 9 * n, n, n };
	dict_rect[(int)Tile::SHADOW6] = {n, 9 * n, n, n };
	dict_rect[(int)Tile::SHADOW7] = { 3 * n, 9 * n, n, n };
	dict_rect[(int)Tile::SHADOWDOWN] = { 4 * n, 9 * n, n, n };
	dict_rect[(int)Tile::SHADOWOTRA] = { 12 * n, 9 * n, n, n };
	dict_rect[(int)Tile::SHADOWOTRA2] = { 2 * n, 8 * n, n, n };

	dict_rect[(int)Tile::FLOOR] = { 3 * n, 13 * n, n, n };
	dict_rect[(int)Tile::FLOORSTART] = { n, 13 * n, n, n };


	dict_rect[(int)Tile::LADDER_L] = { 2 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LADDER_R] = { 3 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LADDER_TOP_L] = { 4 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LADDER_TOP_R] = { 5 * n, 2 * n, n, n };

	dict_rect[(int)Tile::LOCK_RED] = { 6 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LOCK_YELLOW] = { 7 * n, 2 * n, n, n };

	dict_rect[(int)Tile::LASER_L] = { 0, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_R] = { 4 * n, 6 * n, n, n };
	
	dict_rect[(int)Tile::LASER_FRAME0] = { 1 * n, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_FRAME1] = { 2 * n, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_FRAME2] = { 3 * n, 6 * n, n, n };

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/tilesLVL1.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	laser = new Sprite(img_tiles);
	if (laser == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	laser->SetNumberAnimations(1);
	laser->SetAnimationDelay(0, ANIM_DELAY);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	laser->SetAnimation(0);

	return AppStatus::OK;
}
UI::UI()
{
	scene = nullptr;
	width = 0;
	height = 0;
	Letters = nullptr;

	InitUIDictionary();
}

UI::~UI()
{
	if (scene != nullptr)
	{
		delete[] scene;
		scene = nullptr;
	}
}

void UI::InitUIDictionary()
{
	const int n = LETTER_SIZE;
	const int y = LETTER_DIS_Y;
	const int x = LETTER_DIS_X;
	int pos = 0;

	for (float i = 0; i < 4; i++)
	{
		for (float j = 0; j < 59; j++)
		{
			dict_rect[(int)UI_elements::EXCL_W + pos] = { j * x, i * y, n, n };
			pos++;
		}
	}

}

AppStatus UI::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_LETTERS, "images/0.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	Letters = data.GetTexture(Resource::IMG_LETTERS);

	return AppStatus::OK;
}
AppStatus UI::Load(int data[], int w, int h)
{
	size = w * h;
	width = w;
	height = h;

	if (scene != nullptr)	delete[] scene;

	scene = new UI_elements[size];
	if (scene == nullptr)
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(scene, data, size * sizeof(int));

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{
	laser->Update();
}
void UI::Update()
{

}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y*width;
	if(idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::AIR;
	}
	return map[x + y * width];
}
UI_elements UI::GetTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
			return UI_elements::NT;
	}
	return scene[x + y * width];
}
bool TileMap::IsTileStatic(Tile tile) const
{
	return (Tile::STATIC_FIRST <= tile && tile <= Tile::STATIC_LAST);
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
bool TileMap::IsTileLaser(Tile tile) const
{
	return (Tile::LASER_FIRST <= tile && tile <= Tile::LASER_LAST);
}
bool TileMap::IsTileMario(Tile tile) const
{
	return (tile == Tile::LOCK_RED);
}
bool TileMap::IsTileFloor(Tile tile) const
{
	return (Tile::FLOOR_FIRST <= tile && tile <= Tile::FLOOR_LAST);
}
bool TileMap::IsTileLadderTop(Tile tile) const
{
	return tile == Tile::LADDER_TOP_L || tile == Tile::LADDER_TOP_R;
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionGround(const AABB& box, int *py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE;
		return true;
	}
	else if (CollisionYFLOOR(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE + TILE_SIZE / 2;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}

bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;
	
	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileMario(GetTileIndex(x, y)) || IsTileStatic(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile) || IsTileLadderTop(tile) ||IsTileMario(tile))
			return true;
	}
	return false;
}
bool TileMap::CollisionYFLOOR(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE + TILE_SIZE/2;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileFloor(tile) || tile == Tile::PLATFORMMIDDLESTART || tile == Tile::PLATFORMMIDDLEFINISH)
		return true;
	}
	return false;
}
bool TileMap::TestCollisionHead(const AABB& box, int *posY) const
{
	int x, y, x0, x1;
	Tile tile;

	y = (box.pos.y - 1) / TILE_SIZE; 
	x0 = box.pos.x / TILE_SIZE;
	x1 = (box.pos.x + box.width - 1) / TILE_SIZE;

	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		if (tile == Tile::LOCK_RED)
			return true;
	}
	return false;
}
bool TileMap::TestCollisionLaser(const AABB& box, int* posY) const
{
	int x, y, x0, x1;
	Tile tile;

	y = (box.pos.y + 16) / TILE_SIZE; 
	x0 = box.pos.x / TILE_SIZE;

	x1 = (box.pos.x + box.width - 1) / TILE_SIZE;

	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		if (IsTileLaser(tile))
			return true;
	}
	return false;
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				if (tile != Tile::LASER)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
				else
				{
					laser->Draw((int)pos.x, (int)pos.y);
				}
			}
		}
	}
}
void UI::Render()
{
	UI_elements tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = scene[i * width + j];
			if (tile != UI_elements::NT)
			{
				pos.x = (float)j * LETTER_SIZE;
				pos.y = (float)i * LETTER_SIZE;

				if (tile != UI_elements::NT)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*Letters, rc, pos, WHITE);
				}
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);

	laser->Release();

	dict_rect.clear();
}
void UI::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_TILES);

	dict_rect.clear();
}