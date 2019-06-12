#include <SDL_image.h>
#include <sstream>
#include "ioMod.h"
#include "gamedata.h"
#include "renderContext.h"
#include "clock.h"
IOmod& IOmod::getInstance() {
  static IOmod instance;
  return instance;
}

IOmod::~IOmod() { 
  TTF_CloseFont(font);
  TTF_Quit(); 
}

IOmod::IOmod() : 
  init(TTF_Init()),
  renderer( RenderContext::getInstance().getRenderer() ),
  font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file").c_str(),
                    Gamedata::getInstance().getXmlInt("font/size"))),
  textColor({0xff, 0, 0, 0})
{
  if ( init == -1 ) {
    throw std::string("error: Couldn't init font");
  }
  if (font == NULL) {
    throw std::string("error: font not found");
  }
  textColor.r = Gamedata::getInstance().getXmlInt("font/red");
  textColor.g = Gamedata::getInstance().getXmlInt("font/green");
  textColor.b = Gamedata::getInstance().getXmlInt("font/blue");
  textColor.a = Gamedata::getInstance().getXmlInt("font/alpha");
}

SDL_Texture* IOmod::readTexture(const std::string& filename) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename.c_str());
  if ( texture == NULL ) {
    throw std::string("Couldn't load ") + filename;
  }
  return texture;
}

SDL_Surface* IOmod::readSurface(const std::string& filename) {
  SDL_Surface *surface = IMG_Load(filename.c_str());
  if ( !surface ) {
    throw std::string("Couldn't load ") + filename;
  }
  return surface;
}

void IOmod::writeText(const std::string& msg, int x, int y) const {
  SDL_Surface* surface = 
    TTF_RenderText_Solid(font, msg.c_str(), textColor);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void IOmod::writeText(const std::string& msg, const SDL_Color& color,
                      int x, int y) const {
  SDL_Surface* surface = 
    TTF_RenderText_Solid(font, msg.c_str(), color);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}
void IOmod::writeName() const {
  writeText("Jacary Richardson", 30, 440);
}

void IOmod::writeFPS(int x, int y) const {
  std::stringstream strm;
  int fps = Clock::getInstance().Clock::getFps();
  strm << "Fps =" << fps;
  std::string fpsd(strm.str() );

  writeText(fpsd, x,y);
}
