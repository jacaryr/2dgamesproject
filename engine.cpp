#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include <thread>
#include "smartSprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "healthBar.h"
#include "frameGenerator.h"
#include "player.h"
#include "collisionStrategy.h"
const SDL_Color yellow = {255, 255, 0, 255};

static bool isdead = false;
static bool menuop = false;
Engine::~Engine() { 
  delete player;
  for ( auto & sprite :  *sprites ) {
    delete sprite;
  }
  delete sprites;
  delete healthbar;
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( &RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world2("back2", Gamedata::getInstance().getXmlInt("back2/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("SpinningStar")),
  sprites(new std::vector<SmartSprite*>),
  strategies(),
  currentStrategy(0),
  collision(false),
  makeVideo( false ),
  healthbar(new HealthBar("HealthBar")),
  huddraw(false),
  sound(),
  menuEngine(),
  menuMsgLoc(
  Vector2f( Gamedata::getInstance().getXmlInt("menu/msg/x"), 
            Gamedata::getInstance().getXmlInt("menu/msg/y") )
          )


{
  int n = Gamedata::getInstance().getXmlInt("numberOfStars");
  //sound();
  sprites->reserve(n);
  player->setScale(.2);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  std::vector<std::thread> threads;
  for (int i = 0; i < n; ++i) {
    sprites->emplace_back( new SmartSprite("YellowStar", pos, w, h) );
    player->attach( sprites->at(i) );
      sound.toggleMusic();

  }

  strategies.push_back( new PerPixelCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}
void Engine::drawpause() const{
  //clock.pause();
  int sec = clock.getInstance().getSeconds();
  std::stringstream strm, strm2;
  world.draw();
  world2.draw();
  io.writeName();
  strm<< "PAUSED at :"<<sec<<" Seconds";
  io.writeText(strm.str(),300,400);
  viewport.draw();
  SDL_RenderPresent(renderer);


  }
void Engine::drawend() const{
  clock.pause();
  int sec = clock.getInstance().getSeconds();
  std::stringstream strm, strm2;
  world.draw();
  world2.draw();
  io.writeName();
  bool won;
  if( (won = sprites->size()==0)) strm << "GOOD JOB YOU SURVIVED" ;
  else strm << "GOOD JOB YOU DIED" ;
  io.writeText(strm.str(),won?250:300,300);
  strm2 << "time survived : " << sec<<" Seconds"; 
  io.writeText(strm2.str(),300,200);
  io.writeText("Press _R_to restart",300,400);
  viewport.draw();
  SDL_RenderPresent(renderer);


  }
void Engine::draw() const {
  world.draw();
  world2.draw();
  io.writeName();
  for (  auto& sprite : *sprites ) {
    std::thread t([&sprite](){sprite->draw();});
    //sprite->draw();
    t.join();
  }

  std::stringstream strm;
  strm << sprites->size() << " Smart Sprites Remaining";
  if(sprites->size()==0)isdead = true;
  strategies[currentStrategy]->draw();
  if ( collision ) {
    IOmod::getInstance().writeText("Oops: Collision", 500, 90);
  }
  if(!huddraw)
  healthbar->draw();
  io.writeText("YOU MAY RUN OUT OF BULLETS BE WARNED!!", 10, 10);
  player->draw();
  viewport.draw();
  SDL_RenderPresent(renderer);
}

bool Engine::checkForCollisions() {
  bool healthloss = false, hit = false;;
  auto it = sprites->begin();
  while ( it != sprites->end() ) {
    if ((hit = strategies[currentStrategy]->execute(*player, **it))) {
      sound[2];

      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = sprites->erase(it);
      healthloss = true;
    //if((isdead = healthbar->isdead())){}
    //else
    {player->explode();}
          
    }
    else
    if((hit = player->hitTarget(*it))){
      sound[4];
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      healthloss = true;
      it = sprites->erase(it);
      
    }
    else ++it;
  
  }
  
  return healthloss;
}

void Engine::update(Uint32 ticks) {
    Vector2f pos = player->getPosition();
  int w = player->getScaledWidth()/2;
  int h = player->getScaledHeight()/2;
  int m = menuop? menuEngine.getNumStars() : 3;
  std::vector<std::thread> threads;
  player->update(ticks);
  if (checkForCollisions()){
    for(int i=0;i<m;i++){
      sprites->emplace_back( new SmartSprite("YellowStar", pos, w, h) );
      player->attach( sprites->back() );
    }
  }
 
  for ( auto& sprite : *sprites ) {
    threads.push_back(std::thread([&](){sprite->update( ticks );}));
  }
  for (auto& t : threads){
    t.join();
  }



  world.update();
  world2.update();
  viewport.update(); // always update viewport last
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;


  while ( !done && !isdead ) {
    // The next loop polls for events, guarding against key bounce:

    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;

          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ){ 
            clock.unpause();
          }
          else {
          clock.pause();  
          drawpause();
          }
        }
        if ( keystate[SDL_SCANCODE_M] || keystate[SDL_SCANCODE_O] ) {
          clock.pause();
          menuEngine.play();
  if ( menuEngine.starsOptionChosen() ) {
  menuop = true;
  }
  clock.unpause();
}
        if (keystate[SDL_SCANCODE_F1] ) {
          if(huddraw)  huddraw = false;
          else huddraw = true;
        }

        
         if (keystate[SDL_SCANCODE_R]) {
        if (clock.isPaused())clock.startClock();
        return true;
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        (player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        (player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        (player)->up();
      }
     
      if (keystate[SDL_SCANCODE_S]) {
        (player)->down();
      }

      if (keystate[SDL_SCANCODE_SPACE]) {
        (player)->shoot();
      }
  draw();
  update(ticks);
   
  if ( makeVideo ) {
        clock.pause();
        frameGen.makeFrame();
        clock.unpause();
      }
    }   
  }
  //draw ending screen
  if (isdead){ sound[2];
    sound.toggleMusic();
  }
  while (isdead&&!done)
    while ( SDL_PollEvent(&event) ) {
            drawend();
      
      keystate = SDL_GetKeyboardState(NULL);
      if(event.type == SDL_KEYDOWN) {
      if ( makeVideo ) {
        clock.pause();
        /*for (int i =0;i<;i++)*/frameGen.makeFrame();
        clock.unpause();
      }

      if (event.type ==  SDL_QUIT) { done = true; break; }
        if (keystate[SDL_SCANCODE_R]) {
        clock.startClock();
        clock.unpause();

        done = false;
        isdead = false;
        return true;
        }
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
        
          break;
        }
      }
      

 }
 return false;
}
