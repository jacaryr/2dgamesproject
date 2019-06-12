#include "player.h"
#include "gamedata.h"
#include "smartSprite.h"

Player::~Player( ) {
  delete explosion;
}

Player::Player( const std::string& name) :
  Runner(name),
  collision(false),
  initialVelocity(getVelocity()),
  observers(),
  bulletName("bullet"),
  bulletinterval(Gamedata::getInstance().getXmlInt("bullet/interval")),
  timeSinceLastShot(),
  minBullVelo(Vector2f(Gamedata::getInstance().getXmlInt("bullet/speed/x"),
              Gamedata::getInstance().getXmlInt("bullet/speed/x"))),
  bullets("bullet"),
  Up(false),
  Down(false),
  Left(false),
  Right(false),
  Stop(false),
  explosion(nullptr)
{ }

Player::Player(const Player& s) :
  Runner(s), 
  collision(s.collision),
  initialVelocity(s.getVelocity()),
  observers(s.observers),
  bulletName(s.bulletName),
  bulletinterval( s.bulletinterval),
  timeSinceLastShot(s.timeSinceLastShot),
  minBullVelo(s.minBullVelo),
  bullets(s.bulletName),
  Up(false),
  Down(false),
  Left(false),
  Right(false),
  Stop(false),
  explosion(s.explosion)
  { }

Player& Player::operator=(const Player& s) {
  Runner::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  observers = s.observers;
  bulletName = (s.bulletName);
  bulletinterval = ( s.bulletinterval);
  timeSinceLastShot = (s.timeSinceLastShot);
  minBullVelo = (s.minBullVelo);
  bullets =  (s.bullets);
    Up=(false);
  Down=(false);
  Left=(false);
  Right=(false);
  Stop =(false);
  explosion =(s.explosion);
  return *this;
}

void Player::stop() { 
  //setVelocity( Vector2f(0, 0) );
  Stop = true;
  setVelocityX( 0.53*getVelocityX() );
  setVelocityY(0.53*getVelocityY());
}

void Player::right() { 
    Right = true;
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]*1.1);
  }
} 
void Player::left()  { 
    Left = true;
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]*1.1);
  }
} 
void Player::up()    { 
    Up = true;
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1]*1.1);
  }
} 
void Player::down()  { 
    Down = true;
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1]*1.1);
  }
}
void Player::draw()const{
    if ( explosion ) {
    explosion->draw();
    return;
  }
  Runner::draw();
  bullets.draw();
}

void Player::explode() { 
  if ( !explosion ) {
    Sprite 
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    sprite.setScale( getScale() );
    explosion = new ExplodingSprite(sprite);
    collision = true;
  }
}


void Player::shoot(){
      std::cout<<"about to shoot"<<timeSinceLastShot<<std::endl;

  //if (timeSinceLastShot>=bulletinterval){
    timeSinceLastShot = 0;

    Vector2f vel = Vector2f(0,0);//getVelocity();
    float x = getX() + getScaledWidth()/4 ;
    float y = getY() + getScaledHeight()/4 ;
    
    
      
      //vel[0] += (vel[0]>0)? minBullVelo[0]:-minBullVelo[0];
      // if (Stop&&(!Up||!Down||!Left||!Right)) {
     // vel[0] = minBullVelo[0] ;
     // Stop = false;    
      //return;
       //}
       
       
       
    if (Up&&(!Left||!Right)) {
      vel[1] -= minBullVelo[1] ;
      Up = false;    
    }
    
    if (Down&&(!Left||!Right)) {
      vel[1] += minBullVelo[1] ;
      Down = false;    
    }
    
    if (Right&&(!Up||!Down)) {
      vel[0] += minBullVelo[0] ;
      Right = false;    
    }
    
    if (Left&&(!Up||!Down)) {
      vel[0] -= minBullVelo[0] ;
      Left = false;    
    } 
   // else   
 
    
      
    if ((vel[0]!=0||vel[1]!=0)){
    bullets.shoot( Vector2f(x, y), vel ); 
    }
}


void Player::update(Uint32 ticks) {


    if ( explosion ) {
    explosion->update(ticks);

    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    bullets.update(ticks);

    return;
  }else Runner::update(ticks);
  timeSinceLastShot += ticks;

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    int offset = rand()%5;
    offset = offset*(rand()%2?-1:1);
    const Vector2f off(offset, offset);
    (*ptr)->setPlayerPos( getPosition()+off );
    ++ptr;
  }
      bullets.update(ticks);

   stop();
}
void Player::detach(SmartSprite* o ){
  collision=true;
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

