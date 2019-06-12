#include<sstream>
#include "healthBar.h"
#include "renderContext.h"
#include "clock.h"

HealthBar::HealthBar(const std::string& name) :  
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),
  renderer(RenderContext::getInstance().getRenderer()),
  barRect( { Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
             Gamedata::getInstance().getXmlInt(name+"/startLoc/y"),
             Gamedata::getInstance().getXmlInt(name+"/width"),
             Gamedata::getInstance().getXmlInt(name+"/height")}
  ),
  hudRect( { Gamedata::getInstance().getXmlInt(name+"/hudLoc/x"),
             Gamedata::getInstance().getXmlInt(name+"/hudLoc/y"),
             Gamedata::getInstance().getXmlInt(name+"/hWidth"),
             Gamedata::getInstance().getXmlInt(name+"/hHeight")}
  ),
  clock( Clock::getInstance() ),
  currentLength(barRect.w), 
  increment(barRect.w/Gamedata::getInstance().getXmlInt(name+"/increment")),
  interval(Gamedata::getInstance().getXmlInt(name+"/interval")),
  deltaTime(0),
  barColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/barColor/a")}
  ),
  backColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/backColor/a")}
  ),
  hudColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/hudColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hudColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hudColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/hudColor/a")}
  ),
  textColor(
    {(Uint8)Gamedata::getInstance().getXmlInt(name+"/textColor/r"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/textColor/g"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/textColor/b"),
     (Uint8)Gamedata::getInstance().getXmlInt(name+"/textColor/a")}
  ),
  io( IOmod::getInstance() )
{}
bool HealthBar::isdead() {
  if ( currentLength > 0  ) {
    currentLength -= increment;
    return false;
  }
  else { return true;} 
 

}

void HealthBar::draw() const {

  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // Set the health bar background color:
  SDL_SetRenderDrawColor( 
    renderer, hudColor.r, hudColor.g, hudColor.b, hudColor.a );
  // Draw the  background
  SDL_RenderFillRect( renderer, &hudRect );
  SDL_SetRenderDrawColor( 
    renderer, backColor.r, backColor.g, backColor.b, backColor.a );
  // Draw the  background
  SDL_RenderFillRect( renderer, &barRect );

  SDL_SetRenderDrawColor( 
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  int x = static_cast<int>(getX());
  int y = static_cast<int>(getY());
  SDL_Rect healthRect = {x, y, currentLength, barRect.h};
  SDL_RenderFillRect( renderer, &healthRect );
  io.writeText("health :", textColor, x-80, y-7);
  // Set the  color for the health bar outline:
  SDL_SetRenderDrawColor( 
    renderer, barColor.r, barColor.g, barColor.b, barColor.a );
  SDL_RenderDrawRect( renderer, &barRect );
  io.writeFPS(30,60);
  io.writeText("AVOID THE SEA OF COOL! SURVIVE!!", textColor, 30, 90);
  io.writeText("How To Move: A:left D:right W:up S:down", textColor,30, 170);
  io.writeText("How To Shoot: SpaceBar + Movement", textColor,30, 190);
  int sec = clock.getInstance().getSeconds();
  std::stringstream strm;
  strm <<"time : " << sec;
  io.writeText(strm.str(),30 ,140);
}
