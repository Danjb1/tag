#include "TimeUtils.h"

////////////////////////////////////////////////////////////////////////////////
// Windows
////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#pragma comment(lib, "winmm.lib")
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <timeapi.h>
#endif  // _WIN32

////////////////////////////////////////////////////////////////////////////////
// Common
////////////////////////////////////////////////////////////////////////////////

#include <GLFW/glfw3.h>

#include <chrono>
#include <thread>

namespace TimeUtils {

// Conversion factors
static constexpr int millisPerSecond = 1000;
static constexpr int nanosPerMs = 1000000;

/** Maximum time that we are happy to spend spinning, in seconds. */
static constexpr float maxSpinTime = 3.f / millisPerSecond;

/** Maximum time that we are happy to spend yielding, in seconds. */
static constexpr float maxYieldTime = 10.f / millisPerSecond;

////////////////////////////////////////////////////////////////////////////////
// Windows
////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32

PrecisionTimer::PrecisionTimer()
{
    // Set timer resolution
    TIMECAPS tc;
    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR)
    {
        timerResolution = tc.wPeriodMin;
        timeBeginPeriod(timerResolution);
    }

    // Create a timer
    timer = CreateWaitableTimer(NULL, TRUE, NULL);
}

PrecisionTimer::~PrecisionTimer()
{
    // Reset timer resolution
    if (timerResolution > 0)
    {
        timeEndPeriod(timerResolution);
    }

    // Clean up resources
    if (timer)
    {
        CloseHandle(timer);
    }
}

bool nanosleep(HANDLE timer, LONGLONG ns)
{
    if (!timer)
    {
        return false;
    }

    // Set timer properties
    LARGE_INTEGER li {};
    li.QuadPart = -ns;
    if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE))
    {
        return false;
    }

    // Start and wait for timer
    WaitForSingleObject(timer, INFINITE);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Fallback implementation
////////////////////////////////////////////////////////////////////////////////

#else

PrecisionTimer::PrecisionTimer() {}

PrecisionTimer::~PrecisionTimer() {}

#endif

////////////////////////////////////////////////////////////////////////////////
// Cross-platform
////////////////////////////////////////////////////////////////////////////////

void PrecisionTimer::wait(float waitTime)
{
    double startTime = glfwGetTime();
    double timeElapsed = 0.0;
    double timeRemaining = waitTime;

    while (timeRemaining > 0.0)
    {
        // If there is hardly any time to wait, just spin until done.
        // This is the most reliable way to kill time but uses max CPU.
        if (timeRemaining < maxSpinTime)
        {
            // Do nothing at all - even a print statement here can
            // throw off our timing.
        }

        // If there is a little time to wait, yield to other threads.
        // Note that this will still max out the CPU.
        else if (timeRemaining < maxYieldTime)
        {
            std::this_thread::yield();
        }

        // If we have to wait any longer than that, sleep to give the CPU a break
        else
        {
            // Sleep for the shortest possible duration, just to be safe.
            // This can still cause frame drops, as sleep may take longer than requested.
            sleep(1);
        }

        double nowTime = glfwGetTime();
        timeElapsed = nowTime - startTime;
        timeRemaining = waitTime - timeElapsed;
    }
}

void PrecisionTimer::sleep(long ns)
{
    // We have to rely on OS-specific methods for high-precision sleeps.
    // See: https://stackoverflow.com/a/41862592/1624459

#ifdef _WIN32
    if (nanosleep(timer, ns))
    {
        return;
    }
#endif

    // Default cross-platform sleep, if all else fails
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

}  // namespace TimeUtils
