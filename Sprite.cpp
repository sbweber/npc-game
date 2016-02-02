// Samuel Weber

#include "Sprite.h"


Sprite::Sprite(SDL_Renderer *ren, const string &spriteFile, const string &n,
        const string &p, const string &scriptFile)
{
  spriteSheet = loadTexture(spriteFile, ren);
  sprite = DOWN;  // default all sprites as facing down
  facing = SOUTH;
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
}  // Sprite::Sprite(SDL_Renderer *ren, const string &spriteFile)


Sprite::~Sprite()
{
  SDL_DestroyTexture(spriteSheet);
}  // Sprite::~Sprite()


void Sprite::clearActs()
{
  while (get<1>(popAct()) != BAD_ACTION);
}  // void Sprite::clearMoves()


bool Sprite::decSpline()
{
  if (spline > 0)
  {
    if (facing == NORTH || facing == SOUTH)
      spline -= (TILE_HEIGHT / NUM_FRAMES_SPLINE);
    else
      spline -= (TILE_WIDTH / NUM_FRAMES_SPLINE);
    if (spline < 0)
      spline = 0;
    return true;
  }
  return false;
}  // void Sprite::decSpline()


void Sprite::changeDir(dir d)
{
  facing = d;
  switch (d)
  {
  case NORTH:
    sprite = UP;
    break;
  case SOUTH:
    sprite = DOWN;
    break;
  case WEST:
    sprite = LEFT;
    break;
  case EAST:
    sprite = RIGHT;
    break;
  default:
    break;  // default should be impossible
  }  // changes Sprite direction
}  // void Sprite::changeDir(dir d)


dir Sprite::getFacing()
{
  return facing;
}  // dir Sprite::getFacing()


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
  return action(UNDEFINED_DIRECTION, BAD_ACTION);
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
      quit("Told to quit while in text", 0, ren);
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

