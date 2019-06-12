#ifndef HEALTHBAR_H
#define HEALTHBAR_H 
#include "SDL.h"
#include "gamedata.h"
#include "clock.h"
#include "drawable.h"
#include "ioMod.h"

class HealthBar : public Drawable {
public:
  HealthBar(const std::string&);
  void draw() const;
  void update(Uint32 t) {int i = 0;i = i +t; return;};
  bool isdead();
  void reset() { currentLength = barRect.w; }

  HealthBar(const HealthBar&) = delete;
  HealthBar& operator=(const HealthBar&) = delete;
  const SDL_Surface* getSurface() const { return NULL;}
  virtual int getScaledWidth() const  { return barRect.w; }
  virtual int getScaledHeight() const { return barRect.h; }
  virtual const Image* getImage() const { 
    throw std::string("No image in HealthBar");
  }
private:
  SDL_Renderer* renderer;
  SDL_Rect barRect;
  SDL_Rect hudRect;
  Clock& clock;
  int currentLength;
  int increment;  
  float interval; // elapsed time for each decrement (1000ms=1sec)
  int deltaTime;
  SDL_Color barColor;
  SDL_Color backColor;
  SDL_Color hudColor;
  SDL_Color textColor;
  IOmod& io;
};
#endif
