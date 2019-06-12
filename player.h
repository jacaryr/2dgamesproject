#ifndef PLAYER__H
#define PLAYER__H

#include "runner.h"
#include "bullets.h"
#include <cmath>
#include <string>
#include <list>
#include "explodingSprite.h"

// In this example the player is derived from MultiSprite.
// However, there are many options.
class SmartSprite;

class Player : public Runner {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);
  
  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);
  void attach( SmartSprite* o){ observers.push_back(o); }
  void detach(SmartSprite* o);
  void explode();
  bool hitTarget(const Drawable* t) {return bullets.collided(t);}
  void draw()const;
  void shoot();
  void right();
  void left();
  void up();
  void down();
  void stop();
  ~Player();
private:
  bool collision;
  Vector2f initialVelocity;
  std::list<SmartSprite*> observers;
   //bullet info
  std::string bulletName;
  float bulletinterval;
  float timeSinceLastShot;
  Vector2f minBullVelo;
  Bullets bullets;
  bool Up;
  bool Down;
  bool Left;
  bool Right;
  bool Stop;
  ExplodingSprite* explosion;

};
#endif
