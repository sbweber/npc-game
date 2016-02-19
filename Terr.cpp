// Samuel Weber

#include "Terr.h"


Terr::Terr(SDL_Renderer *r, const string &str)
{
  w = 0;
  h = 0;
  ren = r;
  // spritesheets

  tileSS.emplace("Carpet", loadTexture("Tiles-Carpet.png", ren));
  tileSS.emplace("Floor", loadTexture("Tiles-Floor.png", ren));
  tileSS.emplace("Throne", loadTexture("Tiles-Throne.png", ren));
  tileSS.emplace("Wall", loadTexture("Tiles-Wall.png", ren));
  if (str.length())
    loadMap(str);  // if string length is 0, there's no map to load.
}  // Terr::Terr(string str)


Terr::~Terr()
{
  SDL_DestroyTexture(tileSS["Carpet"]);
  SDL_DestroyTexture(tileSS["Floor"]);
  SDL_DestroyTexture(tileSS["Throne"]);
  SDL_DestroyTexture(tileSS["Wall"]);
  tileSS.clear();
}  // Terr::~Terr()


string Terr::actSprites(shared_ptr<Sprite> partySprite,
        vector<shared_ptr<Unit> > &enemies)
{
  string message;
  SDL_Event* wait = new SDL_Event();
  shared_ptr<Sprite> sprite;
  vector<shared_ptr<Sprite> > spritesOnMap;
  for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
      if (sprite = getSprite(map[i][j]))
        spritesOnMap.push_back(sprite);
  for (vector<shared_ptr<Sprite> >::iterator itr = spritesOnMap.begin(); itr != spritesOnMap.end(); itr++)
  {
    sprite = *itr;
    if (sprite)
    {
      action act = sprite->popAct();
      switch (get<1>(act))
      {
      case MOVE:
        moveSprite(sprite, get<0>(act));
        if (sprite == partySprite)
          SDL_PushEvent(wait);
        break;
      case INTERACT:
        if (sprite == partySprite)
        {
          message = interactSprite(partySprite, enemies);
          SDL_PushEvent(wait);
        }
        break;
      case BAD_ACTION:
      default:
        break;
      }
    }
  }
  return message;
}  // void Terr::actSprites(shared_ptr<Sprite> partySprite)


void Terr::enterTileMessageHandler(const string &message, shared_ptr<Tile> tile)
{
  size_t strpos = message.find(':');
  if (strpos != string::npos)
  {
    if (message.substr(0, strpos) == "LOAD-MAP")
    {
      shared_ptr<Sprite> sprite = getSprite(tile);
      if (sprite->getPurpose() != "Hero")
      {
        setSprite(sprite, nullptr);
        return;
      }
      sprite->setSpline(0);
      sprite->clearActs();
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
      setSprite(sprite, getTile(destX, destY));
    }
  }  // No colon found: do default behavior (nothing)
}  // Message Handler for entering a tile


bool Terr::findCheckRoute(dir d, unordered_map<shared_ptr<Tile>, int> &tiles,
        shared_ptr<Tile> tile)
{
  if (tile->getTile(d) &&
          tiles.find(tile->getTile(d)) != tiles.end() &&
          (tiles.find(tile->getTile(d))->second <
          tiles.find(tile)->second))
    return true;
  return false;
}  // if tile in dir exists, might be on a route, and is closer, return true.


void Terr::findEnqueue(dir d, priority_queue<tuple<int, shared_ptr<Tile>>,
        vector<tuple<int, shared_ptr<Tile>> >,
        greater<tuple<int, shared_ptr<Tile>> > > &searchQ,
        unordered_map<shared_ptr<Tile>, int> &tiles,
        tuple<int, shared_ptr<Tile>> t, shared_ptr<Tile> target)
{
  shared_ptr<Tile> tile = get<1>(t);
  if (tile->getTile(d) && tiles.find(tile->getTile(d)) == tiles.end() &&
          tile->getTile(d)->getIsPassable() &&
          (!isOccupied(tile->getTile(d)) || tile->getTile(d) == target))
  {
    searchQ.emplace(get<0>(t) + 1 + findDistance(tile->getTile(d), target), tile->getTile(d));
    tiles.emplace(tile->getTile(d), get<0>(t) + 1);
  }
}  // Enqueues unused tile in a direction from current tile


int Terr::findDistance(shared_ptr<Tile> start,
        shared_ptr<Tile> dest)
{
  return (abs(dest->getX() - start->getX()) +
          abs(dest->getY() - start->getY()));
}  // int Terr::findDistance(shared_ptr<Tile> start, shared_ptr<Tile> dest)


void Terr::findPath(shared_ptr<Tile> start, shared_ptr<Tile> dest,
        shared_ptr<Sprite> sprite)
{
  if (!start || !dest || !sprite)
    return;  // Function assumes non-null start/dest/sprite
  priority_queue<tuple<int, shared_ptr<Tile>>,
          vector<tuple<int, shared_ptr<Tile>> >,
          greater<tuple<int, shared_ptr<Tile>> > > searchQ;
  unordered_map<shared_ptr<Tile>, int> tiles;
  searchQ.emplace(0 + findDistance(dest, start), dest);
  tiles.emplace(dest, 0 + findDistance(dest, start));
  bool found = false;
  tuple<int, shared_ptr<Tile>> t;
  while (!searchQ.empty() && !found)
  {
    t = searchQ.top();
    searchQ.pop();
    if (get<1>(t) == start)
      found = true;
    else
    {
      findEnqueue(EAST, searchQ, tiles, t, start);
      findEnqueue(NORTH, searchQ, tiles, t, start);
      findEnqueue(SOUTH, searchQ, tiles, t, start);
      findEnqueue(WEST, searchQ, tiles, t, start);
    }  // enqueue tile's neighbors with a distance 1 greater if valid and new
  }  // While search queue not empty and element not found

  if (!found)
    return;
  sprite->clearActs();
  shared_ptr<Tile> tile = start;
  while (tile != dest)
  {
    if (findCheckRoute(EAST, tiles, tile))
    {
      sprite->pushAct(action(EAST, MOVE));
      tile = tile->getTile(EAST);
    }
    else if (findCheckRoute(NORTH, tiles, tile))
    {
      sprite->pushAct(action(NORTH, MOVE));
      tile = tile->getTile(NORTH);
    }
    else if (findCheckRoute(SOUTH, tiles, tile))
    {
      sprite->pushAct(action(SOUTH, MOVE));
      tile = tile->getTile(SOUTH);
    }
    else if (findCheckRoute(WEST, tiles, tile))
    {
      sprite->pushAct(action(WEST, MOVE));
      tile = tile->getTile(WEST);
    }
  }  // while you haven't gotten back to dest
  if (getSprite(dest))
    sprite->pushAct(action(UNDEFINED_DIRECTION, INTERACT));
}  // void Terr::findPath(shared_ptr<Tile> start, shared_ptr<Tile> dest, shared_ptr<Sprite> sprite)


int Terr::getHeight()
{
  return h;
}  // int Terr::getHeight()


SDL_Renderer* Terr::getRen()
{
  return ren;
}  // SDL_Renderer* Terr::getRen()


shared_ptr<Sprite> Terr::getSprite(shared_ptr<Tile> tile)
{
  if (sprites.right.find(tile) != sprites.right.end())
    return sprites.right.find(tile)->second;
  return nullptr;
}  // shared_ptr<Sprite> Terr::getSprite(shared_ptr<Tile> tile)


shared_ptr<Tile> Terr::getTile(int i, int j)
{
  return map[i][j];
}  // shared_ptr<Tile> Terr::getTile(int i, int j)


shared_ptr<Tile> Terr::getTile(shared_ptr<Sprite> sprite)
{
  if (sprites.left.find(sprite) != sprites.left.end())
    return sprites.left.find(sprite)->second;
  return nullptr;
}  // shared_ptr<Tile> Terr::getTile(shared_ptr<Sprite> sprite)


int Terr::getWidth()
{
  return w;
}  // int Terr::getWidth()


string Terr::interactSprite(shared_ptr<Sprite> sprite,
        vector<shared_ptr<Unit> > &enemies)
{
  shared_ptr<Tile> tile = getTile(sprite);
  if (!tile)
  {
    logError("Sprite not on map!");
    return "";
  }  // Sprite must be on map.
  tile = tile->getTile(sprite->getFacing());
  if (!tile)
    return "";  // not an error, just invalid. Interacting with edge of map.
  if (isOccupied(tile))
    return interactSprites(sprite, getSprite(tile), enemies);
  return "";
}  // void Terr::interactSprite(shared_ptr<Sprite> sprite)


string Terr::interactSprites(shared_ptr<Sprite> sprite,
        shared_ptr<Sprite> target, vector<shared_ptr<Unit> > &enemies)
{
  if (sprite->getPurpose() == "Hero" && target->getPurpose() == "KillTest")
    setSprite(target, nullptr);
  else if (sprite->getPurpose() == "Hero")
    target->say(ren);
  if (sprite->getPurpose() == "Hero" && target->getPurpose() == "HealTest")
    return "PARTY: full-heal";
  if (sprite->getPurpose() == "Hero" && target->getPurpose() == "FightTest")
  {
    enemies.emplace_back(new Unit());
    return "CHANGE-STATE: BATTLE";
  }
  return "";
}  // void Terr::interactSprites(shared_ptr<Sprite> sprite, shared_ptr<Sprite> sprite)


bool Terr::isOccupied(shared_ptr<Tile> tile)
{
  if (getSprite(tile))
    return true;
  return false;
}  // bool Terr::isOccupied(shared_ptr<Tile> tile)


void Terr::loadMap(const string &str)
{
  ifstream file;
  int c;
  file.open(("resources/maps/"+str), ifstream::in);
  if (!file.good())
    logSDLError("Map not found: " + str);  // error, probably bad filename

  sprites.clear();
  file >> w;
  file >> h;
  map.resize(w);
  for (vector <vector<shared_ptr<Tile> > >::iterator itr = begin(map); itr != end(map);
          itr++)
    itr->resize(h);
  for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
      map[i][j].reset(new Tile(tileSS["Wall"], false));
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
        map[i][j]->setTex(tileSS["Wall"]);
        map[i][j]->setIsPassable(false);
        break;
      case ' ':
        map[i][j]->setTex(tileSS["Floor"]);
        map[i][j]->setIsPassable(true);
        break;
      case 'c':
        map[i][j]->setTex(tileSS["Carpet"]);
        map[i][j]->setIsPassable(true);
        break;
      case 't':
        map[i][j]->setTex(tileSS["Throne"]);
        map[i][j]->setIsPassable(true);
        break;
      case '\r':
      case '\f':
      case '\n':
        i--;
        break;
      default:
        map[i][j]->setTex(tileSS["Wall"]);
        map[i][j]->setIsPassable(false);
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
      if (map[i][j]->getTile(EAST) &&
              map[i][j]->getTex() == map[i][j]->getTile(EAST)->getTex())
      {
        E = true;
        adjacent++;
      }
      if (map[i][j]->getTile(NORTH) &&
              map[i][j]->getTex() == map[i][j]->getTile(NORTH)->getTex())
      {
        N = true;
        adjacent++;
      }
      if (map[i][j]->getTile(SOUTH) &&
              map[i][j]->getTex() == map[i][j]->getTile(SOUTH)->getTex())
      {
        S = true;
        adjacent++;
      }
      if (map[i][j]->getTile(WEST) &&
              map[i][j]->getTex() == map[i][j]->getTile(WEST)->getTex())
      {
        W = true;
        adjacent++;
      }
      if (adjacent == 3)
      {
        if (!N)
          map[i][j]->setType(TILE_EDGE_N);
        else if (!E)
          map[i][j]->setType(TILE_EDGE_E);
        else if (!S)
          map[i][j]->setType(TILE_EDGE_S);
        else if (!W)
          map[i][j]->setType(TILE_EDGE_W);
      }
      else if (adjacent == 2)
      {
        if (S && W)
          map[i][j]->setType(TILE_CORNER_NE);
        else if (E && S)
          map[i][j]->setType(TILE_CORNER_NW);
        else if (W && N)
          map[i][j]->setType(TILE_CORNER_SE);
        else if (N && E)
          map[i][j]->setType(TILE_CORNER_SW);
        else if (E && W)
          map[i][j]->setType(TILE_THIN_EW);
        else if (N && S)
          map[i][j]->setType(TILE_THIN_NS);
      }
      else if (adjacent == 1)
      {
        if (E)
          map[i][j]->setType(TILE_END_E);
        else if (N)
          map[i][j]->setType(TILE_END_N);
        else if (S)
          map[i][j]->setType(TILE_END_S);
        else if (W)
          map[i][j]->setType(TILE_END_W);
      }
      else if (adjacent == 0)
        map[i][j]->setType(TILE_ISOLATED);
    }

  // Set up special Tiles
  while (file.good())
  {
    char instruction = ' ';
    file >> instruction;
    switch (instruction)
    {
    case 'N':
      loadSprite(file);
      break;
    case 'W':
      loadWarpTile(file);
      break;
    default:
      break;  // default: do nothing
    }
  }  // Until EOF hit, all remaining info is warps
  file.close();
}  // void Terr::loadMap(string str)


void Terr::loadSprite(ifstream &file)
{
  int x, y;
  string spriteFile = "NPC.png", name = "I_AM_ERROR";
  string purpose = "ERROR", scriptFile = "Silence.txt"; 
  file >> x;
  file >> y;
  file >> spriteFile;
  file >> name;
  file >> purpose;
  file >> scriptFile;
  shared_ptr<Sprite> sprite(new Sprite(ren, 1, 5, spriteFile, name, purpose,
          scriptFile));
  setSprite(sprite, getTile(x, y));
}  // void Terr::loadSprite(ifstream &file)


void Terr::loadWarpTile(ifstream &file)
{
  int sourceX, sourceY, destX, destY;
  string destTerr;
  file >> sourceX;
  file >> sourceY;
  file >> destTerr;
  file >> destX;
  file >> destY;
  if (sourceX <= w && sourceX >= 0 && sourceY <= h && sourceY >= 0)
  {
    shared_ptr<Tile> tile = map[sourceX][sourceY];
    map[sourceX][sourceY].reset(new Warp(map[sourceX][sourceY], destTerr,
            destX, destY));
    if (map[sourceX][sourceY]->getTile(EAST) &&
            map[sourceX][sourceY]->getTile(EAST)->getTile(WEST) == tile)
      map[sourceX][sourceY]->getTile(EAST)->connectTile(WEST,
              map[sourceX][sourceY]);
    if (map[sourceX][sourceY]->getTile(NORTH) &&
            map[sourceX][sourceY]->getTile(NORTH)->getTile(SOUTH) == tile)
      map[sourceX][sourceY]->getTile(NORTH)->connectTile(SOUTH,
              map[sourceX][sourceY]);
    if (map[sourceX][sourceY]->getTile(SOUTH) &&
            map[sourceX][sourceY]->getTile(SOUTH)->getTile(NORTH) == tile)
      map[sourceX][sourceY]->getTile(SOUTH)->connectTile(NORTH,
              map[sourceX][sourceY]);
    if (map[sourceX][sourceY]->getTile(WEST) &&
            map[sourceX][sourceY]->getTile(WEST)->getTile(EAST) == tile)
      map[sourceX][sourceY]->getTile(WEST)->connectTile(EAST,
              map[sourceX][sourceY]);
  }  // Tile doesn't upgrade if it's not on the map.
}  // void Terr::loadWarpTile(ifstream &file)


void Terr::moveSprite(shared_ptr<Sprite> sprite, dir d)
{
  if (!sprite)
  {
    logError("Asked to move Sprite that doesn't exist!");
    return;
  }  // Make sure sprite exists!

  sprite->changeDir(d);
  shared_ptr <Tile> currTile(getTile(sprite));
  if (!currTile)
  {
    logError("Asked to move Sprite that isn't on the map!");
    return;
  }  // Make sure sprite has a tile!

  shared_ptr <Tile> targetTile(currTile->getTile(d));
  if (!targetTile || !targetTile->getIsPassable() || isOccupied(targetTile))
    return;  // Not an error, just an invalid move.

  setSprite(sprite, targetTile);
  if (d == NORTH || d == SOUTH)
    sprite->setSpline(TILE_HEIGHT);
  else
    sprite->setSpline(TILE_WIDTH);
  enterTileMessageHandler(targetTile->enterTile(), targetTile);
}  // void Terr::moveSprite(shared_ptr<Sprite> sprite, dir d)


void Terr::setSprite(shared_ptr<Sprite> sprite, shared_ptr<Tile> tile)
{
  if (getSprite(tile))
  {
    shared_ptr<Sprite> spr = getSprite(tile);
    sprites.left.erase(spr);
    sprites.insert(location(spr, nullptr));
  }  // If there's already a Sprite there, override it.
  sprites.left.erase(sprite);
  sprites.insert(location(sprite, tile));
}  // void Terr::setSprite(shared_ptr<Sprite> sprite, shared_ptr<Tile> tile)


void Terr::setTile(shared_ptr<Tile> tile, shared_ptr<Sprite> sprite)
{
  setSprite(sprite, tile);
}  // void Terr::setTile(shared_ptr<Tile> tile, shared_ptr<Sprite> sprite)


void Terr::tickSprites(mt19937_64& rng)
{
  for (location loc : sprites)
  {
    shared_ptr<Sprite> sprite = loc.get_left();
    sprite->decTicks(rng);
  }
}  // void Terr::tickSprites()


shared_ptr<Tile> Terr::tileClick(SDL_MouseButtonEvent &click, shared_ptr<Sprite> sprite)
{
  shared_ptr<Tile> tile = nullptr;
  int x = click.x / TILE_WIDTH - NUM_TILES_WIDTH / 2 + getTile(sprite)->getX();
  int y = click.y / TILE_HEIGHT - NUM_TILES_HEIGHT / 2 + getTile(sprite)->getY();
  if (x >= 0 && x < w && y >= 0 && y < h)
    tile = getTile(x, y);
  return tile;
}  // shared_ptr<Tile> Terr::tileClick(SDL_MouseButtonEvent &click, shared_ptr<Sprite> sprite)

