// Samuel Weber

#include "Sprite.h"


Sprite::Sprite(SDL_Renderer *ren, int min, int max, const string &spriteFile,
        int initTicks, const string &n, const string &p,
        const string &scriptFile)
{
  spriteSheet = loadTexture(spriteFile, ren);
  sprite = SPRITE_DOWN;  // default all sprites as facing down
  facing = DIR_SOUTH;
  spline = 0;
  purpose = p;
  name = n;
  font = TTF_OpenFont("resources/fonts/ClearSans-Light.ttf", 20);
  if (font == nullptr)
    quit("TTF_OpenFont", 5);
  if (!scriptFile.empty())
  {
    ifstream script("resources/scripts/" + scriptFile);
    string line;
    while (script.good())
    {
      getline(script, line);
      speech.push(line);
    }  // copy character's script into memory
  }  // load up speech with the script
  moveFreqMax = max;
  moveFreqMin = min;
  ticks = initTicks;
}  // Sprite::Sprite(SDL_Renderer *ren, const string &spriteFile)


Sprite::~Sprite()
{
  SDL_DestroyTexture(spriteSheet);
}  // Sprite::~Sprite()


void Sprite::changeDir(dir d)
{
  facing = d;
  switch (d)
  {
  case DIR_NORTH:
    sprite = SPRITE_UP;
    break;
  case DIR_SOUTH:
    sprite = SPRITE_DOWN;
    break;
  case DIR_WEST:
    sprite = SPRITE_LEFT;
    break;
  case DIR_EAST:
    sprite = SPRITE_RIGHT;
    break;
  default:
    break;  // default should be impossible
  }  // changes Sprite direction
}  // void Sprite::changeDir(dir d)


void Sprite::clearActs()
{
  while (get<1>(popAct()) != ACT_UNDEFINED);
}  // void Sprite::clearMoves()


void Sprite::decTicks(mt19937_64& randNumGen)
{
  if (ticks)
    ticks--;
  if (!ticks && (moveFreqMin > 0))
  {
    ticks = unsigned int(rng(randNumGen, moveFreqMin, moveFreqMax));
    if (actionQ.empty())
      pushAct(action(randDir(randNumGen), ACT_MOVE));
  }
}  // void Sprite::decTicks(mt19937_64& rng)


dir Sprite::getFacing()
{
  return facing;
}  // dir Sprite::getFacing()


int Sprite::getQSize()
{
  return actionQ.size();
}  // int Sprite::getQSize()


int Sprite::getSpline()
{
  return spline;
}  // int Sprite::getSpline()


spriteType Sprite::getSprite()
{
  return sprite;
}  // SpriteType Sprite::getSprite()


SDL_Texture* Sprite::getSpriteSheet()
{
  return spriteSheet;
}  // SDL_Texture* Sprite::getSpriteSheet()


const string Sprite::getPurpose()
{
  return purpose;
}  // const string Sprite::getType()


const action Sprite::popAct()
{
  if (!actionQ.empty())
  {
    action retval = actionQ.front();
    actionQ.pop();
    return retval;
  }  // If the queue isn't empty, pop the front off and return it.
  return action(DIR_UNDEFINED, ACT_UNDEFINED);
}  // const dir Sprite::popMove()


void Sprite::pushAct(action act)
{
  actionQ.push(act);
}  // void Sprite::pushMove()


void Sprite::renderSpeech(SDL_Renderer *ren, const string &str,
        SDL_Color color)
{
  string temp = str;
  while (!temp.empty())
  {
    renderTextbox(ren, font, "");
    if (!name.empty())
      temp = renderTextbox(ren, font, name + ": " + temp, color);
    else
      temp = renderTextbox(ren, font, temp, color);
    SDL_RenderPresent(ren);
    SDL_Event e = pressAnyKey();
    if (e.type == SDL_KEYDOWN && e.key.keysym == stateQuit)
      eventQuit();
  }
}  // void renderSpeech()


void Sprite::say(SDL_Renderer *ren)
{
  if(speech.empty())
    renderSpeech(ren, "I have nothing more to say.");
  else
  {
    string str = speech.front();
    speech.pop();
    renderSpeech(ren, str);
    speech.push(str);
  }
}  // void Sprite::say(SDL_Renderer *ren, string &str)


void Sprite::setMoveFreq(int min, int max)
{
  moveFreqMax = max;
  moveFreqMin = min;
}  // void Sprite::setMoveFreq(int min, int max)


void Sprite::setPurpose(const string &p)
{
  purpose = p;
}  // void Sprite::setType(string str)


void Sprite::setSpline(int s)
{
  spline = s;
}  // void Sprite::setSpline(int s)


void Sprite::setSprite(spriteType st)
{
  sprite = st;
}  // void Sprite::setSprite()


const action Sprite::topAct()
{
  if (!actionQ.empty())
  {
    action retval = actionQ.front();
    return retval;
  }  // If the queue isn't empty, pop the front off and return it.
  return action(DIR_UNDEFINED, ACT_UNDEFINED);
}  // const dir Sprite::topAct()


bool Sprite::walk()
{
  if (spline > 0)
  {
    if (facing == DIR_NORTH || facing == DIR_SOUTH)
      spline -= (TILE_HEIGHT / NUM_FRAMES_SPLINE);
    else
      spline -= (TILE_WIDTH / NUM_FRAMES_SPLINE);
    if (spline < 0)
      spline = 0;
    switch (sprite)
    {  // Magic Number (/ 4): frames to hold each sprite in walk cycle.
      // Magic Number (% 2): number of sprites in walk cycle.
    case SPRITE_UP:
    case SPRITE_WALK_UP:
      if (((spline / (TILE_HEIGHT / NUM_FRAMES_SPLINE)) / 4) % 2)
        sprite = SPRITE_WALK_UP;
      else
        sprite = SPRITE_UP;
      break;
    case SPRITE_DOWN:
    case SPRITE_WALK_DOWN:
      if (((spline / (TILE_HEIGHT / NUM_FRAMES_SPLINE)) / 4) % 2)
        sprite = SPRITE_WALK_DOWN;
      else
        sprite = SPRITE_DOWN;
      break;
    case SPRITE_LEFT:
    case SPRITE_WALK_LEFT:
      if (((spline / (TILE_WIDTH / NUM_FRAMES_SPLINE)) / 4) % 2)
        sprite = SPRITE_WALK_LEFT;
      else
        sprite = SPRITE_LEFT;
      break;
    case SPRITE_RIGHT:
    case SPRITE_WALK_RIGHT:
      if (((spline / (TILE_WIDTH / NUM_FRAMES_SPLINE)) / 4) % 2)
        sprite = SPRITE_WALK_RIGHT;
      else
        sprite = SPRITE_RIGHT;
      break;
    }
    return true;
  }
  return false;
}  // void Sprite::decSpline()

