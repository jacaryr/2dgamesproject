#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "healthBar.h"
#include "sound.h"
#include "menuEngine.h"

class CollisionStrategy;
class SmartSprite;
class SubjectSprite;
class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World world;
  World world2;
  Viewport& viewport;

  Player* player;
  std::vector<SmartSprite*> *sprites;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  bool collision;

  bool makeVideo;
  HealthBar *healthbar;
  void draw() const;
  void drawend() const;
  void update(Uint32);
  void drawpause() const;
  bool checkForCollisions();
  bool huddraw;
  SDLSound sound;
  MenuEngine menuEngine;
  Vector2f menuMsgLoc;


};
