#ifndef RUNNER__H
#define RUNNER__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
class Runner : public Drawable {
public:
  Runner(const std::string&);
  Runner(const Runner&);

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
  void shoot();
protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
 
  void advanceFrame(Uint32 ticks);
  Runner& operator=(const Runner&);
};
#endif
