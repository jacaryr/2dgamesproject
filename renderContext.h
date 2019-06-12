#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H
#include <iostream>
#include <SDL.h>
#include "imageFactory.h"
const int WIDTH = 854;
const int HEIGHT = 480;
const int DELAY = 1000;
class RenderContext{
public:
  ~RenderContext();
  SDL_Window* getWindow() const { return window; }
  SDL_Renderer* getRenderer() const { return renderer; }

  //SDL_Surface* getSurface(const std::string& n) { 
    //return factory->getSurface(n);
  //}

  Image* getImage(const std::string& ) ;
  
  std::vector<Image*> getImages(const std::string& ) ;
  static RenderContext& getInstance(){
    static RenderContext instance;
    return instance;
    }

  
  RenderContext(const RenderContext&) = delete;
  RenderContext& operator=(const RenderContext&) = delete;
private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  ImageFactory* factory;


  SDL_Window* initWindow();
  SDL_Renderer* initRenderer();
  RenderContext();
};
#endif
