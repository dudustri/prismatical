#pragma once

#include <SDL2/SDL.h>
#include <cstdint>
#include <vector>
#include <memory>

struct SDLWindowDeleter   { void operator()(SDL_Window*   w) { SDL_DestroyWindow(w);   } };
struct SDLRendererDeleter { void operator()(SDL_Renderer* r) { SDL_DestroyRenderer(r); } };
struct SDLTextureDeleter  { void operator()(SDL_Texture*  t) { SDL_DestroyTexture(t);  } };

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    bool init();
    void clear(uint8_t r, uint8_t g, uint8_t b);
    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void drawLine(int x0, int y0, int x1, int y1, int width, uint8_t r, uint8_t g, uint8_t b);
    void present();

    int width()  const { return width_; }
    int height() const { return height_; }

private:
    void drawDot(int x, int y, int radius, uint8_t r, uint8_t g, uint8_t b);

    int  width_;
    int  height_;

    std::unique_ptr<SDL_Window,   SDLWindowDeleter>   window_;
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> sdl_renderer_;
    std::unique_ptr<SDL_Texture,  SDLTextureDeleter>  texture_;

    std::vector<uint8_t> pixels_;
};
