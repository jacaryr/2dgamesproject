#include "runner.h"
#include "gamedata.h"
#include "renderContext.h"

void Runner::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Runner::Runner( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance().getImages(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))

{ }

Runner::Runner(const Runner& s) :
  Drawable(s), 
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )

   { }

Runner& Runner::operator=(const Runner& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );

  return *this;
}






void Runner::draw() const { 
  if(getVelocityY()<=0 && getVelocityX()<=0)
  images[currentFrame%(numberOfFrames/4)]->draw(getX(), getY(), getScale());
  else
  if(getVelocityY()<=0 && getVelocityX()>=0)
  images[currentFrame%(numberOfFrames/4)+4]->draw(getX(), getY(), getScale());
  else
  if(getVelocityY()>=0 && getVelocityX()<=0)
  images[currentFrame%(numberOfFrames/4)+8]->draw(getX(), getY(), getScale());
  else
  if(getVelocityY()>=0 && getVelocityX()>=0)
  images[currentFrame%(numberOfFrames/4)+12]->draw(getX(), getY(), getScale());
}

void Runner::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.01;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( 0 );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    //setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( 0 );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( 0 );
  }  

}
