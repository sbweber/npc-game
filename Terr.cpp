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


int Terr::getHeight()
{
  return h;
}  // int Terr::getHeight()


Sprite* Terr::getSprite(Tile* tile)
{
  return sprites.right.find(tile)->second;
}  // Sprite* Terr::getSprite(Tile* tile)


Tile* Terr::getTile(int i, int j)
{
  return map[i][j];
}  // Tile* Terr::getTile(int i, int j)


Tile* Terr::getTile(Sprite* sprite)
{
  return sprites.left.find(sprite)->second;
}  // Tile* Terr::getTile(Sprite* sprite)


int Terr::getWidth()
{
  return w;
}  // int Terr::getWidth()


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


void Terr::setSprite(Sprite* sprite, Tile* tile)
{
  sprites.left.erase(sprite);
  sprites.insert(location(sprite, tile));
}  // void Terr::setSprite(Sprite* sprite, Tile* tile)


void Terr::setTile(Tile* tile, Sprite* sprite)
{
  setSprite(sprite, tile);
}  // void Terr::setTile(Tile* tile, Sprite* sprite)

