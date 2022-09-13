#pragma once

namespace TimeUtils {

/** Desired frames per second for the game logic. */
static constexpr int fps = 60;

/** Time represented by each frame, in seconds. */
static constexpr float frameTime = 1.f / fps;

/**
 * Maximum number of updates that we will perform between renders, in the case of lag.
 *
 * After this, the game will appear to slow down.
 */
static constexpr int maxUpdatesPerRender = 5;

/**
 * While in scope, forces the lowest possible timer resolution.
 *
 * See:
 * https://docs.microsoft.com/en-gb/windows/win32/multimedia/obtaining-and-setting-timer-resolution
 */
class PrecisionTimer
{
public:
    PrecisionTimer();
    ~PrecisionTimer();

    /** Waits for some time (seconds) using the most appropriate mechanism available. */
    void wait(float waitTime);

    /** Sleeps for the given number of nanoseconds. */
    void sleep(long ns);

private:
    /** Timer handle used for sleep operations. */
    void* timer;

    /** The timer resolution in use. */
    unsigned int timerResolution = 0;
};

}  // namespace TimeUtils
