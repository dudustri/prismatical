#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

class Input {
public:
    Input();
    ~Input();

    void poll();

    bool        quitRequested() const { return quit_; }
    bool        seedRequested() const { return seed_; }
    std::string pendingSeed()   const { return pending_seed_; }

private:
    bool        quit_;
    bool        seed_;
    std::string pending_seed_;

    std::thread       stdin_thread_;
    std::mutex        mutex_;
    std::atomic<bool> running_;
    std::string       thread_seed_;
    std::atomic<bool> seed_ready_;

    // pipe used to unblock the stdin thread on shutdown
    // pipe_fds_[0] = read end, pipe_fds_[1] = write end
    // writing one byte to [1] wakes up select() in stdinLoop
    int pipe_fds_[2];

    void stdinLoop();
    void pollSDL();
};
