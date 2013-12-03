#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <memory>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
*  Window management class, provides a simple wrapper around
*  the SDL_Window and SDL_Renderer functionalities
*/
class Window
{
   public:

      /**
       *  Initialize SDL, setup the window and renderer
       *  @param title The window title
       */
      static void init(std::string title = "Window");

      /**
       * Quit SDL and destroy the window and renderer
       */
      static void quit();

      /**
       *  Draw a SDL_Texture to the screen at dstRect with various other options
       *  @param tex The SDL_Texture to draw
       *  @param dstRect The destination position and width/height to draw the texture with
       *  @param clip The clip to apply to the image, if desired
       *  @param angle The rotation angle to apply to the texture, default is 0
       *  @param xPivot The x coordinate of the pivot, relative to (0, 0) being center of dstRect
       *  @param yPivot The y coordinate of the pivot, relative to (0, 0) being center of dstRect
       *  @param flip The flip to apply to the image, default is none
       */
      static void draw(SDL_Texture        *tex,
                       SDL_Rect           &dstRect,
                       SDL_Rect           *clip  = NULL,
                       float              angle  = 0.0,
                       int                xPivot = 0,
                       int                yPivot = 0,
                       SDL_RendererFlip   flip   = SDL_FLIP_NONE);

      static void drawShip(SDL_Rect &dstRect,
                           int      color,
                           bool     selected);

      static void drawPoint(double x,  double y);


      /**
       *  Loads an image directly to texture using SDL_image's
       *  built in function IMG_LoadTexture
       *  @param file The image file to load
       *  @return SDL_Texture* to the loaded texture
       */
      static SDL_Texture* loadImage(const std::string &file);

      /**
       *  Load a font
       *  @param fontFile The font we want to use to render the text
       *  @param fontSize The size we want the font to be
       *  @return An TTF_Font* to the loaded font
       */
       static TTF_Font* loadFont(const std::string  &fontFile,
                                 int                fontSize);


      /**
       *  Generate a texture containing the message we want to display
       *  @param message The message we want to display
       *  @param font The font we want to use to render the text
       *  @param color The color we want the text to be
       *  @param wrapLength The length to wrap into new lines
       *  @return An SDL_Texture* to the rendered message
       */
       static SDL_Texture* renderText(const std::string  &message,
                                      TTF_Font           *font,
                                      SDL_Color          color,
                                      int                wrapLength = 0);

       /**
       * Clear the renderer
       */
       static void clear();

       /**
       * Present the renderer, ie. update screen
       */
       static void present();

       /**
       * Get the window's box
       */
       static SDL_Rect getBox();

   private:
       static std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> window;
       static std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> renderer;
       static SDL_Rect box;
};

#endif
