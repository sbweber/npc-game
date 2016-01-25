// Samuel Weber

#include "Terr.h"


Terr::Terr(const string &str)
{
  w = 0;
  h = 0;
  if (str.length())
    loadMap(str);  // if string length is 0, there's no map to load.
}  // Terr::Terr(string str)


Terr::~Terr()
{
  for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
      if (map[i][j])
        delete(map[i][j]);
}  // Terr::~Terr()


void Terr::enterTileMessageHandler(const string &message, Tile* tile)
{
  size_t strpos = message.find(':');
  if (strpos != string::npos)
  {
    if (message.substr(0, strpos) == "LOAD-MAP")
    {
      getSprite(tile)->setSpline(0);
      strpos = message.find(' ', strpos);
      size_t strposnew = message.find(' ', strpos + 1);
      string destTerr = message.substr(strpos + 1, strposnew - strpos - 1);
      strpos = strposnew;
      strposnew = message.find(' ', strpos + 1);
      int destX = stoi(message.substr(strpos + 1, strposnew - strpos - 1));
      strpos = strposnew;
      strposnew = message.find(' ', strpos + 1);
      int destY = stoi(message.substr(strpos + 1, strposnew - strpos - 1));
      loadMap(destTerr);
      setSprite(getSprite(tile), getTile(destX, destY));
    }
  }  // No colon found: do default behavior (nothing)
}  // void Terr::enterTileMessageHandler(const string &message, Tile* tile)


bool Terr::findCheckRoute(dir d, unordered_map<Tile*, int> *tiles, Tile* tile)
{
  if (tile->getTile(d) &&
          tiles->find(tile->getTile(d)) != tiles->end() &&
          (tiles->find(tile->getTile(d))->second <
          tiles->find(tile)->second))
    return true;
  return false;
}  // if tile in dir exists, might be on a route, and is closer, return true.


void Terr::findEnqueue(dir d, queue<tuple<Tile*, int> > *searchQ,
  unordered_map<Tile*, int> *tiles, tuple<Tile*, int> t)
{
  Tile* tile = get<0>(t);
  if (tile->getTile(d) && tiles->find(tile->getTile(d)) == tiles->end() &&
    tile->getTile(d)->getIsPassable())
  {
    searchQ->emplace(tile->getTile(d), get<1>(t) +1);
    tiles->emplace(tile->getTile(d), get<1>(t)+1);
  }
}  // Enqueues unused tile in a direction from current tile


dir Terr::findHint(Tile* start, Tile* dest)
{
  int sx = start->getX();
  int sy = start->getX();
  int dx = dest->getX();
  int dy = dest->getY();
  if (start == dest || (dx == sx && dy == sy))
    return UNDEFINED_DIRECTION;  // same tile
  if (abs(dx - sx) > abs(dy - sy))
  {
    if (dx > sx)
      return EAST;
    return WEST;
  }  // if horizontal distance greater than vertical
  else
  {
    if (dy > sy)
      return NORTH;
    return SOUTH;
  }
}  // void Terr::findHint(Tile* start, Tile* dest)


void Terr::findPath(Tile* start, Tile* dest, Sprite* sprite)
{
  if (!start || !dest || !sprite)
    return;  // Function assumes non-null start/dest/sprite
  queue<tuple<Tile*, int> > searchQ;
  unordered_map<Tile*, int> tiles;
  searchQ.emplace(dest, 0);
  tiles.emplace(dest, 0);
  bool found = false;
  tuple<Tile*, int> t;
  while (!searchQ.empty() && !found)
  {
    t = searchQ.front();
    searchQ.pop();
    if (get<0>(t) == start)
      found = true;
    else
    {
      findEnqueue(EAST, &searchQ, &tiles, t);
      findEnqueue(NORTH, &searchQ, &tiles, t);
      findEnqueue(SOUTH, &searchQ, &tiles, t);
      findEnqueue(WEST, &searchQ, &tiles, t);
    }  // enqueue tile's neighbors with a distance 1 greater if valid and new
  }  // While search queue not empty and element not found

  if (!found)
    return;
  sprite->clearMoves();
  Tile* tile = start;
  while (tile != dest)
  {
    if (findCheckRoute(EAST, &tiles, tile))
    {
      sprite->pushMove(EAST);
      tile = tile->getTile(EAST);
    }
    else if (findCheckRoute(NORTH, &tiles, tile))
    {
      sprite->pushMove(NORTH);
      tile = tile->getTile(NORTH);
    }
    else if (findCheckRoute(SOUTH, &tiles, tile))
    {
      sprite->pushMove(SOUTH);
      tile = tile->getTile(SOUTH);
    }
    else if (findCheckRoute(WEST, &tiles, tile))
    {
      sprite->pushMove(WEST);
      tile = tile->getTile(WEST);
    }
  }  // while you haven't gotten back to dest
}  // void Terr::findPath(Tile* start, Tile* dest, Sprite* sprite)


int Terr::getHeight()
{
  return h;
}  // int Terr::getHeight()


Sprite* Terr::getSprite(Tile* tile)
{
  if (sprites.right.find(tile) != sprites.right.end())
    return sprites.right.find(tile)->second;
  return nullptr;
}  // Sprite* Terr::getSprite(Tile* tile)


Tile* Terr::getTile(int i, int j)
{
  return map[i][j];
}  // Tile* Terr::getTile(int i, int j)


Tile* Terr::getTile(Sprite* sprite)
{
  if (sprites.left.find(sprite) != sprites.left.end())
    return sprites.left.find(sprite)->second;
  return nullptr;
}  // Tile* Terr::getTile(Sprite* sprite)


int Terr::getWidth()
{
  return w;
}  // int Terr::getWidth()


void Terr::interactSprite(Sprite* sprite)
{
  Tile* tile = getTile(sprite);
  if (!tile)
  {
    logError("Sprite not on map!");
    return;
  }  // Sprite must be on map.
  tile = tile->getTile(sprite->getFacing());
  if (!tile)
    return;  // not an error, just invalid. Interacting with edge of map.
  if (isOccupied(tile))
    interactSprites(sprite, getSprite(tile));
}  // void Terr::interactSprite(Sprite* sprite)


void Terr::interactSprites(Sprite* sprite, Sprite* target)
{
  if (sprite->getType() == "Hero" && target->getType() == "test")
  {
    setSprite(target, nullptr);
  }  // Test interaction: Hero kills test NPC (removes from map).
}  // void Terr::interactSprites(Sprite* sprite, Sprite* sprite)


bool Terr::isOccupied(Tile* tile)
{
  if (getSprite(tile))
    return true;
  return false;
}  // bool Terr::isOccupied(Tile* tile)


void Terr::loadMap(const string &str)
{
  ifstream file;
  int c;
  file.open(("resources/"+str), ifstream::in);
  if (!file.good())
    logSDLError("Map not found.");  // error, probably bad filename

  file >> w;
  file >> h;
  map.resize(w);
  for (vector <vector<Tile*> >::iterator itr = begin(map); itr != end(map);
          itr++)
    itr->resize(h);
  for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
      map[i][j] = new Tile(VOID);
  for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
    {
      map[i][j]->setPos(i, j);
      if (i > 0)
        map[i][j]->connectTile(WEST, map[i - 1][j]);
      if (i < (w - 1))
        map[i][j]->connectTile(EAST, map[i + 1][j]);
      if (j > 0)
        map[i][j]->connectTile(NORTH, map[i][j - 1]);
      if (j < (h - 1))
        map[i][j]->connectTile(SOUTH, map[i][j + 1]);
    }  // assumes a rectangular map

  // intentionally nesting like this, despite slower loop, because map is
  // currently read in that manner. Due to small maps, ease of understanding
  // is more valuable than efficient memory parsing.
  for (int j = 0; j < h; j++)
    for (int i = 0; i < w; i++)
    {
      c = file.get();
      if (!file.good())
        return;  // file read error
      switch (c)
      {
      case 'x':
        map[i][j]->setType(VOID);
        break;
      case ' ':
        map[i][j]->setType(OPEN);
        break;
      case 'c':
        map[i][j]->setType(CARPET_CORE);
        // first pass: specify all carpets as core.
        // Determine Flip status on next pass through the map's tiles, below.
        break;
      case 't':
        map[i][j]->setType(THRONE);
        break;
      case '\r':
      case '\f':
      case '\n':
        i--;
        break;
      default:
        map[i][j]->setType(VOID);
        break;
      }
    }

  // intelligent sprite selection from correct spritesheet,
  // based on adjacent tiles (corner piece? edge? etc)
  bool N = false, S = false, E = false, W = false;
  int adjacent = 0;
  for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
    {
      adjacent = 0;
      N = S = E = W = false;
      switch (map[i][j]->getType())
      {
      case CARPET_CORE:
        if (map[i][j]->getTile(NORTH) &&
                (map[i][j]->getTile(NORTH)->getType() >= CARPET_CORE) &&
                (map[i][j]->getTile(NORTH)->getType() <= CARPET_EDGE_S))
        {
          N = true;
          adjacent++;
        }  // range check, only works if all carpet types are consecutive
        if (map[i][j]->getTile(SOUTH) &&
                (map[i][j]->getTile(SOUTH)->getType() >= CARPET_CORE) &&
                (map[i][j]->getTile(SOUTH)->getType() <= CARPET_EDGE_S))
        {
          S = true;
          adjacent++;
        }
        if (map[i][j]->getTile(EAST) &&
                (map[i][j]->getTile(EAST)->getType() >= CARPET_CORE) &&
                (map[i][j]->getTile(EAST)->getType() <= CARPET_EDGE_S))
        {
          E = true;
          adjacent++;
        }
        if (map[i][j]->getTile(WEST) &&
                (map[i][j]->getTile(WEST)->getType() >= CARPET_CORE) &&
                (map[i][j]->getTile(WEST)->getType() <= CARPET_EDGE_S))
        {
          W = true;
          adjacent++;
        }
        if (adjacent == 3)
        {
          if (!N)
            map[i][j]->setType(CARPET_EDGE_N);
          else if (!E)
            map[i][j]->setType(CARPET_EDGE_E);
          else if (!S)
            map[i][j]->setType(CARPET_EDGE_S);
          else if (!W)
            map[i][j]->setType(CARPET_EDGE_W);
        }
        else if (adjacent == 2)
        {
          if (N && E)
            map[i][j]->setType(CARPET_CORNER_SW);
          else if (E && S)
            map[i][j]->setType(CARPET_CORNER_NW);
          else if (S && W)
            map[i][j]->setType(CARPET_CORNER_NE);
          else if (W && N)
            map[i][j]->setType(CARPET_CORNER_SE);
        }
        break;
      default:
        break;
      }
    }

  // Set up Warp Tiles
  while (file.good())
  {
    int sourceX, sourceY, destX, destY;
    string destTerr;
    
    file >> sourceX;
    file.ignore(numeric_limits<streamsize>::max(), ' ');
    file >> sourceY;
    file.ignore(numeric_limits<streamsize>::max(), ' ');
    file >> destTerr;
    file.ignore(numeric_limits<streamsize>::max(), ' ');
    file >> destX;
    file.ignore(numeric_limits<streamsize>::max(), ' ');
    file >> destY;
    if (sourceX <= w && sourceX >= 0 && sourceY <= h && sourceY >= 0)
      map[sourceX][sourceY] = new Warp(map[sourceX][sourceY], destTerr,
              destX, destY, true);
  }  // Until EOF hit, all remaining info is warps
  file.close();
}  // void Terr::loadMap(string str)


void Terr::moveSprite(Sprite* sprite, dir d)
{
  if (!sprite)
  {
    logError("Asked to move Sprite that doesn't exist!");
    return;
  }  // Make sure sprite exists!

  sprite->changeDir(d);
  Tile *currTile = getTile(sprite);
  if (!currTile)
  {
    logError("Asked to move Sprite that isn't on the map!");
    return;
  }  // Make sure sprite has a tile!

  Tile *targetTile = currTile->getTile(d);
  if (!targetTile || !targetTile->getIsPassable() || isOccupied(targetTile))
    return;  // Not an error, just an invalid move.

  setSprite(sprite, targetTile);
  if (d == NORTH || d == SOUTH)
    sprite->setSpline(TILE_HEIGHT);
  else
    sprite->setSpline(TILE_WIDTH);
  enterTileMessageHandler(targetTile->enterTile(), targetTile);
}  // void Terr::moveSprite(Sprite* sprite, dir d)


void Terr::setSprite(Sprite* sprite, Tile* tile)
{
  if (getSprite(tile))
  {
    Sprite* spr = getSprite(tile);
    sprites.left.erase(spr);
    sprites.insert(location(spr, nullptr));
  }  // If there's already a Sprite there, override it.
  sprites.left.erase(sprite);
  sprites.insert(location(sprite, tile));
}  // void Terr::setSprite(Sprite* sprite, Tile* tile)


void Terr::setTile(Tile* tile, Sprite* sprite)
{
  setSprite(sprite, tile);
}  // void Terr::setTile(Tile* tile, Sprite* sprite)


Tile* Terr::tileClick(SDL_MouseButtonEvent &click, Sprite* sprite)
{
  Tile* tile = nullptr;
  int x = click.x / TILE_WIDTH - NUM_TILES_WIDTH / 2 + getTile(sprite)->getX();
  int y = click.y / TILE_HEIGHT - NUM_TILES_HEIGHT / 2 + getTile(sprite)->getY();
  if (x >= 0 && x < w && y >= 0 && y < h)
    tile = getTile(x, y);
  return tile;
}  // Tile* Terr::tileClick(SDL_MouseButtonEvent &click, Sprite* sprite)

