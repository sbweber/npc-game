// Samuel Weber

#include "Terrain.h"


Terrain::Terrain(SDL_Renderer *r, mt19937_64 &randNumGen, const string &str)
{
  w = 0;
  h = 0;
  ren = r;
  // spritesheets

  tileSS.emplace("Carpet", loadTexture("Tiles-Carpet.png", ren));
  tileSS.emplace("Floor", loadTexture("Tiles-Floor.png", ren));
  tileSS.emplace("Forest", loadTexture("Tiles-Forest.png", ren));
  tileSS.emplace("Throne", loadTexture("Tiles-Throne.png", ren));
  tileSS.emplace("Wall", loadTexture("Tiles-Wall.png", ren));
  if (str.length())
    loadMap(str, randNumGen); // if string length is 0, there's no map to load.
} // Terr::Terr(string str)


Terrain::~Terrain()
{
  SDL_DestroyTexture(tileSS["Carpet"]);
  SDL_DestroyTexture(tileSS["Floor"]);
  SDL_DestroyTexture(tileSS["Forest"]);
  SDL_DestroyTexture(tileSS["Throne"]);
  SDL_DestroyTexture(tileSS["Wall"]);
  tileSS.clear();
} // Terr::~Terr()


string Terrain::actSprite(shared_ptr<Sprite> partySprite,
        shared_ptr<Sprite> sprite, vector<shared_ptr<Unit> > &enemies)
{
  string message;
  if (sprite && !sprite->getSpline())
  {
    action act = sprite->popAct();
    switch (get<1>(act))
    {
      case ACT_MOVE:
        if (sprite == partySprite)
        {
          if (get<0>(act) != sprite->getFacing() && sprite->getQSize())
            moveSprite(sprite, get<0>(act));
        }
        else if (get<0>(act) != sprite->getFacing())
          moveSprite(sprite, get<0>(act));
        message = moveSprite(sprite, get<0>(act));
        break;
      case ACT_INTERACT:
        if (sprite == partySprite)
          message = interactSprite(partySprite, enemies);
        break;
      case ACT_UNDEFINED:
      default:
        break;
    }
  }
  return message;
} // string Terr::actSprite()


string Terrain::actSprites(shared_ptr<Sprite> partySprite,
        vector<shared_ptr<Unit> > &enemies)
{
  string message;
  queue<shared_ptr<Sprite> > spritesOnMap;
  for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
      if (getSprite(map[i][j]))
        spritesOnMap.push(getSprite(map[i][j]));
  while (!spritesOnMap.empty())
  {
    shared_ptr<Sprite> actingSprite = spritesOnMap.front();
    spritesOnMap.pop();
    string m = actSprite(partySprite, actingSprite, enemies);
    if (actingSprite == partySprite)
      message = m;
    if ((actingSprite != partySprite) && (m.substr(0, m.find(':')) == "LOAD-STATE_MAP"))
      setSprite(nullptr, getTile(actingSprite));
  }
  return message;
} // void Terr::actSprites(shared_ptr<Sprite> partySprite)


bool Terrain::findCheckRoute(DIR d, unordered_map<shared_ptr<Tile>, int> &tiles,
        shared_ptr<Tile> tile)
{
  if (tile->getTile(d) &&
          tiles.find(tile->getTile(d)) != tiles.end() &&
          (tiles.find(tile->getTile(d))->second <
          tiles.find(tile)->second))
    return true;
  return false;
} // if tile in dir exists, might be on a route, and is closer, return true.


void Terrain::findEnqueue(DIR d, priority_queue<tuple<int, shared_ptr<Tile>>,
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
} // Enqueues unused tile in a direction from current tile


int Terrain::findDistance(shared_ptr<Tile> start,
        shared_ptr<Tile> dest)
{
  return (abs(dest->getX() - start->getX()) +
          abs(dest->getY() - start->getY()));
} // int Terr::findDistance(shared_ptr<Tile> start, shared_ptr<Tile> dest)


void Terrain::findPath(shared_ptr<Tile> start, shared_ptr<Tile> dest,
        shared_ptr<Sprite> sprite)
{
  if (!start || !dest || !sprite)
    return; // Function assumes non-null start/dest/sprite
  priority_queue<tuple<int, shared_ptr<Tile>>,
          vector<tuple<int, shared_ptr < Tile>> >,
          greater<tuple<int, shared_ptr < Tile>> > > searchQ;
  unordered_map<shared_ptr<Tile>, int> tiles;
  searchQ.emplace(0 + findDistance(dest, start), dest);
  tiles.emplace(dest, 0 + findDistance(dest, start));
  bool found = false;
  tuple<int, shared_ptr < Tile>> t;
  while (!searchQ.empty() && !found)
  {
    t = searchQ.top();
    searchQ.pop();
    if (get<1>(t) == start)
      found = true;
    else
    {
      findEnqueue(DIR_EAST, searchQ, tiles, t, start);
      findEnqueue(DIR_NORTH, searchQ, tiles, t, start);
      findEnqueue(DIR_SOUTH, searchQ, tiles, t, start);
      findEnqueue(DIR_WEST, searchQ, tiles, t, start);
    } // enqueue tile's neighbors with a distance 1 greater if valid and new
  } // While search queue not empty and element not found

  if (!found)
    return;
  sprite->clearActs();
  shared_ptr<Tile> tile = start;
  while (tile != dest)
  {
    if (findCheckRoute(DIR_EAST, tiles, tile))
    {
      sprite->pushAct(action(DIR_EAST, ACT_MOVE));
      tile = tile->getTile(DIR_EAST);
    }
    else if (findCheckRoute(DIR_NORTH, tiles, tile))
    {
      sprite->pushAct(action(DIR_NORTH, ACT_MOVE));
      tile = tile->getTile(DIR_NORTH);
    }
    else if (findCheckRoute(DIR_SOUTH, tiles, tile))
    {
      sprite->pushAct(action(DIR_SOUTH, ACT_MOVE));
      tile = tile->getTile(DIR_SOUTH);
    }
    else if (findCheckRoute(DIR_WEST, tiles, tile))
    {
      sprite->pushAct(action(DIR_WEST, ACT_MOVE));
      tile = tile->getTile(DIR_WEST);
    }
  } // while you haven't gotten back to dest
  if (getSprite(dest))
    sprite->pushAct(action(DIR_UNDEFINED, ACT_INTERACT));
  sprite->pushAct(action(DIR_UNDEFINED, ACT_UNDEFINED));
} // void Terr::findPath(shared_ptr<Tile> start, shared_ptr<Tile> dest, shared_ptr<Sprite> sprite)


int Terrain::getHeight()
{
  return h;
} // int Terr::getHeight()


SDL_Renderer* Terrain::getRen()
{
  return ren;
} // SDL_Renderer* Terr::getRen()


shared_ptr<Sprite> Terrain::getSprite(shared_ptr<Tile> tile)
{
  return tile->getSprite();
} // shared_ptr<Sprite> Terr::getSprite(shared_ptr<Tile> tile)


shared_ptr<Tile> Terrain::getTile(int i, int j)
{
  return map[i][j];
} // shared_ptr<Tile> Terr::getTile(int i, int j)


shared_ptr<Tile> Terrain::getTile(shared_ptr<Sprite> sprite)
{
  for (vector<shared_ptr<Tile> > row : map)
    for (shared_ptr<Tile> tile : row)
      if (tile->getSprite() == sprite)
        return tile;
  return nullptr;
} // shared_ptr<Tile> Terr::getTile(shared_ptr<Sprite> sprite)


int Terrain::getWidth()
{
  return w;
} // int Terr::getWidth()


string Terrain::interactSprite(shared_ptr<Sprite> sprite,
        vector<shared_ptr<Unit> > &enemies)
{
  shared_ptr<Tile> tile = getTile(sprite);
  if (!tile)
  {
    logError("Sprite not on map!");
    return "";
  } // Sprite must be on map.
  tile = tile->getTile(sprite->getFacing());
  if (!tile)
    return ""; // not an error, just invalid. Interacting with edge of map.
  if (isOccupied(tile))
    return interactSprites(sprite, getSprite(tile), enemies);
  return "";
} // void Terr::interactSprite(shared_ptr<Sprite> sprite)


string Terrain::interactSprites(shared_ptr<Sprite> sprite,
        shared_ptr<Sprite> target, vector<shared_ptr<Unit> > &enemies)
{
  if (sprite->getPurpose() == "Hero")
    target->say(ren);
  if (sprite->getPurpose() == "Hero" && target->getPurpose() == "HealTest")
    return "PARTY: full-heal";
  else if (sprite->getPurpose() == "Hero" && target->getPurpose() == "FightTest")
  {
    enemies.emplace_back(new Unit("Guard 1", 10, 10, 10, 10, 10, 1000, 1000));
    enemies.emplace_back(new Unit("Guard 2", 10, 10, 10, 10, 10, 1000, 1000));
    enemies.emplace_back(new Unit("Guard 3", 10, 10, 10, 10, 10, 1000, 1000));
    enemies.emplace_back(new Unit("Guard 4", 10, 10, 10, 10, 10, 1000, 1000));
    return "CHANGE-STATE: STATE_BATTLE";
  }
  else if (sprite->getPurpose() == "Hero" && target->getPurpose() == "KillTest")
  {
    shared_ptr<Tile> tile = nullptr;
    for (vector<shared_ptr<Tile> > tvec : map)
      for (shared_ptr<Tile> t : tvec)
        if (t->enterTile().find("LOAD-STATE_MAP:") != string::npos)
          tile = t;
    findPath(getTile(target), tile, target);
    target->setMoveFreq(0, 0);
  }
  return "";
} // void Terr::interactSprites(shared_ptr<Sprite> sprite, shared_ptr<Sprite> sprite)


bool Terrain::isOccupied(shared_ptr<Tile> tile)
{
  if (getSprite(tile))
    return true;
  return false;
} // bool Terr::isOccupied(shared_ptr<Tile> tile)


void Terrain::loadMap(const string &str, mt19937_64 &randNumGen)
{
  ifstream file;
  int c;
  file.open(("resources/maps/" + str), ifstream::in);
  if (!file.good())
    logSDLError("Map not found: " + str); // error, probably bad filename

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
        map[i][j]->connectTile(DIR_WEST, map[i - 1][j]);
      if (i < (w - 1))
        map[i][j]->connectTile(DIR_EAST, map[i + 1][j]);
      if (j > 0)
        map[i][j]->connectTile(DIR_NORTH, map[i][j - 1]);
      if (j < (h - 1))
        map[i][j]->connectTile(DIR_SOUTH, map[i][j + 1]);
    } // assumes a rectangular map

  // intentionally nesting like this, despite slower loop, because map is
  // currently read in that manner. Due to small maps, ease of understanding
  // is more valuable than efficient memory parsing.
  for (int j = 0; j < h; j++)
    for (int i = 0; i < w; i++)
    {
      c = file.get();
      if (!file.good())
        return; // file read error
      switch (c)
      {
        case 'c':
          map[i][j]->setTex(tileSS["Carpet"]);
          map[i][j]->setIsPassable(true);
          break;
        case ' ':
          map[i][j]->setTex(tileSS["Floor"]);
          map[i][j]->setIsPassable(true);
          break;
        case 'f':
          map[i][j]->setTex(tileSS["Forest"]);
          map[i][j]->setIsPassable(true);
          break;
        case 't':
          map[i][j]->setTex(tileSS["Throne"]);
          map[i][j]->setIsPassable(true);
          break;
        case 'x':
          map[i][j]->setTex(tileSS["Wall"]);
          map[i][j]->setIsPassable(false);
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
      if (map[i][j]->getTile(DIR_EAST) &&
              map[i][j]->getTex() == map[i][j]->getTile(DIR_EAST)->getTex())
      {
        E = true;
        adjacent++;
      }
      if (map[i][j]->getTile(DIR_NORTH) &&
              map[i][j]->getTex() == map[i][j]->getTile(DIR_NORTH)->getTex())
      {
        N = true;
        adjacent++;
      }
      if (map[i][j]->getTile(DIR_SOUTH) &&
              map[i][j]->getTex() == map[i][j]->getTile(DIR_SOUTH)->getTex())
      {
        S = true;
        adjacent++;
      }
      if (map[i][j]->getTile(DIR_WEST) &&
              map[i][j]->getTex() == map[i][j]->getTile(DIR_WEST)->getTex())
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
        loadSprite(file, randNumGen);
        break;
      case 'W':
        loadWarpTile(file);
        break;
      default:
        break; // default: do nothing
    }
  } // Until EOF hit, all remaining info is warps
  file.close();
} // void Terr::loadMap(string str)


void Terrain::loadSprite(ifstream &file, mt19937_64 &randNumGen)
{
  int x, y, moveFreqMin, moveFreqMax, initTicks;
  string mfMin, mfMax;
  string spriteFile = "NPC.png", name = "I_AM_ERROR";
  string purpose = "ERROR", scriptFile = "Silence.txt";
  file >> x;
  file >> y;
  file >> spriteFile;
  file >> name;
  file >> purpose;
  file >> scriptFile;
  file >> mfMin; // in secs
  file >> mfMax; // in secs
  moveFreqMin = int(NUM_TICKS_SEC * stod(mfMin));
  moveFreqMax = int(NUM_TICKS_SEC * stod(mfMax));
  initTicks = int(rng(randNumGen, moveFreqMin, moveFreqMax));
  shared_ptr<Sprite> sprite(new Sprite(ren, moveFreqMin, moveFreqMax,
          spriteFile, initTicks, name, purpose, scriptFile));
  setSprite(sprite, getTile(x, y));
} // void Terr::loadSprite(ifstream &file)


void Terrain::loadWarpTile(ifstream &file)
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
    if (map[sourceX][sourceY]->getTile(DIR_EAST) &&
            map[sourceX][sourceY]->getTile(DIR_EAST)->getTile(DIR_WEST) == tile)
      map[sourceX][sourceY]->getTile(DIR_EAST)->connectTile(DIR_WEST,
            map[sourceX][sourceY]);
    if (map[sourceX][sourceY]->getTile(DIR_NORTH) &&
            map[sourceX][sourceY]->getTile(DIR_NORTH)->getTile(DIR_SOUTH) == tile)
      map[sourceX][sourceY]->getTile(DIR_NORTH)->connectTile(DIR_SOUTH,
            map[sourceX][sourceY]);
    if (map[sourceX][sourceY]->getTile(DIR_SOUTH) &&
            map[sourceX][sourceY]->getTile(DIR_SOUTH)->getTile(DIR_NORTH) == tile)
      map[sourceX][sourceY]->getTile(DIR_SOUTH)->connectTile(DIR_NORTH,
            map[sourceX][sourceY]);
    if (map[sourceX][sourceY]->getTile(DIR_WEST) &&
            map[sourceX][sourceY]->getTile(DIR_WEST)->getTile(DIR_EAST) == tile)
      map[sourceX][sourceY]->getTile(DIR_WEST)->connectTile(DIR_EAST,
            map[sourceX][sourceY]);
  } // Tile doesn't upgrade if it's not on the map.
} // void Terr::loadWarpTile(ifstream &file)


string Terrain::moveSprite(shared_ptr<Sprite> sprite, DIR d)
{
  if (!sprite)
  {
    logError("Asked to move Sprite that doesn't exist!");
    return "";
  } // Make sure sprite exists!
  shared_ptr <Tile> currTile(getTile(sprite));
  if (!currTile)
  {
    logError("Asked to move Sprite that isn't on the map!");
    return "";
  } // Make sure sprite has a tile!

  shared_ptr <Tile> targetTile(currTile->getTile(d));
  if (!targetTile || !targetTile->getIsPassable() || isOccupied(targetTile))
    sprite->changeDir(d); // Not an error, just an invalid move.
  else if (d == sprite->getFacing())
  {
    currTile->clearTile();
    // Remove sprite from previous tile
    setSprite(sprite, targetTile);
    if (d == DIR_NORTH || d == DIR_SOUTH)
      sprite->setSpline(TILE_HEIGHT);
    else
      sprite->setSpline(TILE_WIDTH);
    return targetTile->enterTile();
  }
  sprite->changeDir(d);
  return targetTile->enterTile();
} // void Terr::moveSprite(shared_ptr<Sprite> sprite, dir d)


void Terrain::setSprite(shared_ptr<Sprite> sprite, shared_ptr<Tile> tile)
{
  tile->setSprite(sprite);
  // If there's already a Sprite there, override it.
} // void Terr::setSprite(shared_ptr<Sprite> sprite, shared_ptr<Tile> tile)


void Terrain::setTile(shared_ptr<Tile> tile, shared_ptr<Sprite> sprite)
{
  setSprite(sprite, tile);
} // void Terr::setTile(shared_ptr<Tile> tile, shared_ptr<Sprite> sprite)


void Terrain::tickSprites(mt19937_64& rng)
{
  for (vector<shared_ptr<Tile> > row : map)
    for (shared_ptr<Tile> tile : row)
      if (tile->getSprite() != nullptr)
        tile->getSprite()->decTicks(rng);
} // void Terr::tickSprites()


shared_ptr<Tile> Terrain::tileClick(SDL_MouseButtonEvent &click, shared_ptr<Sprite> sprite)
{
  shared_ptr<Tile> tile = nullptr;
  int x = click.x / TILE_WIDTH - NUM_TILES_WIDTH / 2 + getTile(sprite)->getX();
  int y = click.y / TILE_HEIGHT - NUM_TILES_HEIGHT / 2 + getTile(sprite)->getY();
  if (x >= 0 && x < w && y >= 0 && y < h)
    tile = getTile(x, y);
  return tile;
} // shared_ptr<Tile> Terr::tileClick(SDL_MouseButtonEvent &click, shared_ptr<Sprite> sprite)

