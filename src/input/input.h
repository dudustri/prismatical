#pragma once

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
    bool        togglePhaseRequested()  const { return togglePhase_; }
    bool        toggleColorRequested()  const { return toggleColor_; }
    bool        toggleFreqRequested()   const { return toggleFreq_;  }

private:
    bool        quit_;
    bool        seed_;
    std::string pending_seed_;
    bool        togglePhase_;
    bool        toggleColor_;
    bool        toggleFreq_;

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
