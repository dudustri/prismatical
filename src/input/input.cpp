#include "input.h"
#include <iostream>
#include <unistd.h>      // STDIN_FILENO, pipe(), write(), close()
#include <sys/select.h>  // select() — wait on multiple file descriptors simultaneously


std::atomic<bool> Input::interrupted(false);

void Input::onInterrupt(int) {
    Input::interrupted = true;
}

Input::Input()
    : quit_(false)
    , seed_(false)
    , pending_seed_("")
    , togglePhase_(false)
    , toggleColor_(false)
    , toggleFreq_(false)
    , running_(true)
    , seed_ready_(false)
{
    // create a pipe — two connected file descriptors
    int pipe_ends[2];
    if (pipe(pipe_ends) != 0) {
        std::cerr << "failed to create shutdown pipe\n";
        return;
    }
    shutdown_pipe_read_end_  = pipe_ends[0];
    shutdown_pipe_write_end_ = pipe_ends[1];

    signal(SIGINT,  onInterrupt);
    signal(SIGTERM, onInterrupt);
    signal(SIGHUP,  onInterrupt);

    stdin_thread_ = std::thread([this]() { stdinLoop(); });
}

Input::~Input() {
    running_ = false;
    // write one byte to the write end — this wakes up select() in stdinLoop
    if (write(shutdown_pipe_write_end_, "banana", 1) < 0)
        std::cerr << "failed to signal shutdown pipe\n";
    if (stdin_thread_.joinable())
        stdin_thread_.join();
    close(shutdown_pipe_read_end_);
    close(shutdown_pipe_write_end_);
}

void Input::stdinLoop() {
    while (running_) {
        // select() watches multiple file descriptors at once — blocks until one is ready
        fd_set watched_file_descriptors;
        FD_ZERO(&watched_file_descriptors);
        FD_SET(STDIN_FILENO, &watched_file_descriptors);
        FD_SET(shutdown_pipe_read_end_, &watched_file_descriptors);

        // block until stdin has input OR the shutdown pipe is written to
        int ready_count = select(shutdown_pipe_read_end_ + 1, &watched_file_descriptors, nullptr, nullptr, nullptr);
        if (ready_count <= 0 || !running_) break;

        // if the shutdown pipe fired, the destructor signaled us to exit
        if (FD_ISSET(shutdown_pipe_read_end_, &watched_file_descriptors)) break;

        if (FD_ISSET(STDIN_FILENO, &watched_file_descriptors)) {
            std::string line;
            if (!std::getline(std::cin, line)) break;

            // thats tricky: \033[1A = move cursor up, \033[2K = erase line
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
            if (event.key.keysym.sym == SDLK_ESCAPE) quit_         = true;
            if (event.key.keysym.sym == SDLK_SPACE)  seed_         = true;
            if (event.key.keysym.sym == SDLK_1)      togglePhase_  = true;  // phase drift
            if (event.key.keysym.sym == SDLK_2)      toggleColor_  = true;  // color flow
            if (event.key.keysym.sym == SDLK_3)      toggleFreq_   = true;  // freq morph
        }
    }
}

void Input::poll() {
    quit_         = false;
    seed_         = false;
    pending_seed_ = "";
    togglePhase_  = false;
    toggleColor_  = false;
    toggleFreq_   = false;

    pollSDL();

    if (seed_ready_) {
        std::scoped_lock lock(mutex_);
        pending_seed_ = thread_seed_;
        seed_         = true;
        seed_ready_   = false;
    }
}
