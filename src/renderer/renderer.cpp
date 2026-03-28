#include "renderer.h"
#include <iostream>

Renderer::Renderer(int width, int height)
    : width_(width)
    , height_(height)
    , running_(false)
    , window_(nullptr)
    , sdl_renderer_(nullptr)
    , texture_(nullptr)
    , pixels_(width * height * 4, 0)
{}

Renderer::~Renderer() {
    SDL_Quit();
}

bool Renderer::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    window_.reset(SDL_CreateWindow(
        "prismatical",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width_, height_,
        SDL_WINDOW_SHOWN
    ));
    if (!window_) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        return false;
    }

    sdl_renderer_.reset(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED));
    if (!sdl_renderer_) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        return false;
    }

    texture_.reset(SDL_CreateTexture(
        sdl_renderer_.get(),
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        width_, height_
    ));
    if (!texture_) {
        std::cerr << "SDL_CreateTexture failed: " << SDL_GetError() << "\n";
        return false;
    }

    running_ = true;
    return true;
}

void Renderer::clear(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < width_ * height_; i++) {
        pixels_[i * 4 + 0] = r;
        pixels_[i * 4 + 1] = g;
        pixels_[i * 4 + 2] = b;
        pixels_[i * 4 + 3] = 255;
    }
}

void Renderer::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) return;

    int index = (y * width_ + x) * 4;
    pixels_[index + 0] = r;
    pixels_[index + 1] = g;
    pixels_[index + 2] = b;
    pixels_[index + 3] = 255;
}

void Renderer::present() {
    SDL_UpdateTexture(texture_.get(), nullptr, pixels_.data(), width_ * 4);
    SDL_RenderCopy(sdl_renderer_.get(), texture_.get(), nullptr, nullptr);
    SDL_RenderPresent(sdl_renderer_.get());
}

void Renderer::pollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running_ = false;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running_ = false;
    }
}

bool Renderer::running() const {
    return running_;
}
