#include "input.h"
#include <iostream>
#include <unistd.h>   // STDIN_FILENO, pipe(), write(), close()
#include <sys/select.h>  // select() — wait on multiple file descriptors simultaneously

Input::Input()
    : quit_(false)
    , seed_(false)
    , pending_seed_("")
    , running_(true)
    , seed_ready_(false)
{
    // create a pipe — two connected file descriptors
    // writing to pipe_fds_[1] makes pipe_fds_[0] readable — used to wake up select()
    pipe(pipe_fds_);

    stdin_thread_ = std::thread([this]() { stdinLoop(); });
}

Input::~Input() {
    running_ = false;
    // write one byte to the pipe's write end — this wakes up select() in stdinLoop
    // select() will see pipe_fds_[0] is readable and the loop will exit cleanly
    write(pipe_fds_[1], "x", 1);
    if (stdin_thread_.joinable())
        stdin_thread_.join();
    close(pipe_fds_[0]);
    close(pipe_fds_[1]);
}

void Input::stdinLoop() {
    while (running_) {
        // select() watches multiple file descriptors at once — blocks until one is ready
        fd_set fds;
        FD_ZERO(&fds);                    // clear the set
        FD_SET(STDIN_FILENO, &fds);       // watch stdin
        FD_SET(pipe_fds_[0], &fds);       // watch pipe read end — signals shutdown

        // block until stdin has input OR the pipe is written to (shutdown signal)
        int ready = select(pipe_fds_[0] + 1, &fds, nullptr, nullptr, nullptr);
        if (ready <= 0 || !running_) break;

        // if the pipe fired, it's a shutdown signal — exit the loop
        if (FD_ISSET(pipe_fds_[0], &fds)) break;

        // stdin is ready — read the line
        if (FD_ISSET(STDIN_FILENO, &fds)) {
            std::string line;
            if (!std::getline(std::cin, line)) break;

            // erase the blank line Enter leaves behind using ANSI escape codes
            // \033[1A = move cursor up, \033[2K = erase line
            std::cout << "\033[1A\033[2K" << std::flush;

            if (!line.empty()) {
                std::scoped_lock lock(mutex_);
                thread_seed_ = line;
                seed_ready_  = true;
            }
        }
    }
}

void Input::pollSDL() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) quit_ = true;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) quit_ = true;
            if (event.key.keysym.sym == SDLK_SPACE)  seed_ = true;
        }
    }
}

void Input::poll() {
    quit_         = false;
    seed_         = false;
    pending_seed_ = "";

    pollSDL();

    if (seed_ready_) {
        std::scoped_lock lock(mutex_);
        pending_seed_ = thread_seed_;
        seed_         = true;
        seed_ready_   = false;
    }
}
