#include <string>
#include <stdexcept>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Window.h"
/*
Thanks to Twinklebear !
https://github.com/Twinklebear/TwinklebearDev-Lessons
http://twinklebeardev.blogspot.fr/p/sdl-20-tutorial-index.html
*/



// Initialize the unique_ptr's deleters here
std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> Window::window
   = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);
std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> Window::renderer
   = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)>(nullptr, SDL_DestroyRenderer);

// Other static members
SDL_Rect Window::box;

void Window::init(std::string title)
{
   // Initialize all SDL subsystems
   if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
      throw std::runtime_error("SDL Init Failed");
   if (TTF_Init() == -1)
      throw std::runtime_error("TTF Init Failed");

   // Setup our window size
   box.x = 0;
   box.y = 0;
   box.w = 800;
   box.h = 800;

   // Create our window
   window.reset(SDL_CreateWindow(title.c_str(),
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 box.w,
                                 box.h,
                                 SDL_WINDOW_SHOWN));

   // Make sure it created ok
   if (window == nullptr)
      throw std::runtime_error("Failed to create window");

   // Create the renderer
   renderer.reset(SDL_CreateRenderer(window.get(),
                                     -1,
                                     SDL_RENDERER_ACCELERATED
                                     | SDL_RENDERER_PRESENTVSYNC));

   // Make sure it created ok
   if (renderer == nullptr)
      throw std::runtime_error("Failed to create renderer");
}


void Window::quit()
{
    TTF_Quit();
    SDL_Quit();
}

void Window::draw(SDL_Texture       *tex,
                  SDL_Rect          &dstRect,
                  SDL_Rect          *clip,
                  float             angle,
                  int               xPivot,
                  int               yPivot,
                  SDL_RendererFlip  flip)
{
   // get size if not defined
   SDL_QueryTexture(tex, NULL, NULL, &dstRect.w, &dstRect.h);

   // Convert pivot pos from relative to object's top-left corner
   // to be relative to its center
   xPivot += dstRect.w / 2;
   yPivot += dstRect.h / 2;

   // SDL expects an SDL_Point as the pivot location
   SDL_Point pivot = { xPivot, yPivot };


   // Draw the texture
   SDL_RenderCopyEx(renderer.get(), tex, clip, &dstRect, angle, &pivot, flip);
}

void Window::drawShip(SDL_Rect &dstRect,
                      int      color,
                      bool     selected)
{
   //double x, y;

   /*
   int red   = (rgb >> 16) & 0xFF;
   int green = (rgb >> 8) & 0xFF;
   int blue  = rgb & 0xFF;
   */

   if (selected)
   {
         SDL_Rect posRect = dstRect;
         posRect.x -= 1;
         posRect.y -= 1;
         posRect.h += 2;
         posRect.w += 2;

         SDL_SetRenderDrawColor(renderer.get(),
                                255, 255, 255,
                                SDL_ALPHA_OPAQUE);
         SDL_RenderFillRect(renderer.get(), &posRect);
   }

   // http://wiki.libsdl.org/SDL_SetRenderDrawColor
   SDL_SetRenderDrawColor(renderer.get(),
                          (color >> 16) & 0xFF,
                          (color >> 8) & 0xFF,
                           color & 0xFF,
                           SDL_ALPHA_OPAQUE);

   // http://wiki.libsdl.org/SDL_RenderDrawLine
   // SDL_RenderDrawLine(renderer.get(), 150, 50, 200, 400);

   // http://wiki.libsdl.org/SDL_RenderDrawRect
   // http://wiki.libsdl.org/SDL_RenderFillRect
   SDL_RenderFillRect(renderer.get(), &dstRect);

   // traw orbit
   /*
   if (orbit > 0)
   {
      x = (double)dstRect.x;
      y = (double)dstRect.y;
      orbit *= 1.1;
      SDL_SetRenderDrawColor(renderer.get(),
                             100, 100, 100,
                             SDL_ALPHA_OPAQUE);
      const SDL_Point arr[9] = {
         {(int)(x + orbit * 1.0000) , (int)(y + orbit * 0.0000)},
         {(int)(x + orbit * 0.7071) , (int)(y + orbit * 0.7071)},
         {(int)(x + orbit * 0.0000) , (int)(y + orbit * 1.0000)},
         {(int)(x - orbit * 0.7071) , (int)(y + orbit * 0.7071)},
         {(int)(x - orbit * 1.0000) , (int)(y + orbit * 0.0000)},
         {(int)(x - orbit * 0.7071) , (int)(y - orbit * 0.7071)},
         {(int)(x + orbit * 0.0000) , (int)(y - orbit * 1.0000)},
         {(int)(x + orbit * 0.7071) , (int)(y - orbit * 0.7071)},
         {(int)(x + orbit * 1.0000) , (int)(y + orbit * 0.0000)}
      };
      SDL_RenderDrawLines(renderer.get(), arr, 9);
   }
   */
}

void Window::drawPoint(double x,  double y)
{

   SDL_SetRenderDrawColor(renderer.get(),
                          128, 128, 128,
                          SDL_ALPHA_OPAQUE);

   double delta = 5;
   const SDL_Point arr[6] = {
      {(int)(x - delta) , (int)(y)},
      {(int)(x)         , (int)(y - delta)},
      {(int)(x + delta) , (int)(y)},
      {(int)(x)         , (int)(y + delta)},
      {(int)(x - delta) , (int)(y)},
      {(int)(x + delta) , (int)(y)}
   };
   SDL_RenderDrawLines(renderer.get(), arr, 6);
}

SDL_Texture* Window::loadImage(const std::string &file)
{
   SDL_Texture* tex = nullptr;
   tex = IMG_LoadTexture(renderer.get(), file.c_str());
   if (tex == nullptr)
      throw std::runtime_error("Failed to load image: " + file + IMG_GetError());
   return tex;
}

TTF_Font* Window::loadFont(const std::string &fontFile,
                           int                fontSize)
{
   // Open the font
   TTF_Font *font = nullptr;
   font = TTF_OpenFont(fontFile.c_str(), fontSize);
   if (font == nullptr)
      throw std::runtime_error("Failed to load font: " + fontFile + TTF_GetError());

   return font;
}

SDL_Texture* Window::renderText(const std::string  &message,
                                TTF_Font           *font,
                                SDL_Color          color,
                                int                wrapLength)
{
   if (wrapLength == 0) wrapLength = box.w;

   // Render the message to an SDL_Surface,
   // as that's what TTF_RenderText_X returns
   SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(font, message.c_str(), color, wrapLength);
   SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.get(), surf);

   //Clean up unneeded stuff
   SDL_FreeSurface(surf);
   return texture;
}

void Window::clear()
{
   // Black background
   SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(renderer.get());
}

void Window::present()
{
   SDL_RenderPresent(renderer.get());
}

SDL_Rect Window::getBox()
{
   //Update box to match the current window size
   SDL_GetWindowSize(window.get(), &box.w, &box.h);
   return box;
}
