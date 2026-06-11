#pragma once

#include "../anim.h"
#include <SDL2/SDL.h>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <csignal>

class Input {
public:
    Input();
    ~Input();

    static std::atomic<bool> interrupted;

    void poll();

    bool        quitRequested()         const { return quit_ || interrupted; }
    bool        seedRequested()         const { return seed_; }
    std::string pendingSeed()           const { return pending_seed_; }
    bool        animToggleRequested(Anim a) const { return animToggle_[static_cast<int>(a)]; }
    bool        toggleModeRequested()   const { return toggleMode_;  }

private:
    bool        quit_;
    bool        seed_;
    std::string pending_seed_;
    bool        animToggle_[ANIM_COUNT] = {};  // indexed by Anim, set on keys 1..
    bool        toggleMode_;

    std::thread       stdin_thread_;
    std::mutex        mutex_;
    std::atomic<bool> running_;
    std::string       thread_seed_;
    std::atomic<bool> seed_ready_;

    // pipe used to unblock the stdin thread on shutdown
    int shutdown_pipe_read_end_; // select() watches this; becomes readable when shutdown is signaled
    int shutdown_pipe_write_end_; //destructor writes one byte here to wake up select()

    void stdinLoop();
    void pollSDL();
    static void onInterrupt(int);
};
