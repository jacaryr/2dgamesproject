#ifndef COOLGAME__H
#define COOLGAME__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class CoolGame : public Drawable{
public:
  CoolGame(const std::string&);
  CoolGame(const CoolGame&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldHeight;
  int worldWidth;
  void advanceFrame(Uint32 ticks);
  CoolGame& operator=(const CoolGame&);
  Vector2f makeVelocity(int,int)const ;
};
#endif
