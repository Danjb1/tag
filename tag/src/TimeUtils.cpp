#include "TimeUtils.h"

#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>  //tmp
#include <thread>

namespace TimeUtils {

/** Number of milliseconds in 1 second. */
static constexpr float millisPerSecond = 1000.f;

/** Maximum time that we are happy to spend spinning, in seconds. */
static constexpr float maxSpinTime = 3.f / millisPerSecond;

/** Maximum time that we are happy to spend yielding, in seconds. */
static constexpr float maxYieldTime = 10.f / millisPerSecond;

void wait(float waitTime)
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
            // We may be able to improve on this with OS-specific methods:
            // https://stackoverflow.com/a/41862592/1624459
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        double nowTime = glfwGetTime();
        timeElapsed = nowTime - startTime;
        timeRemaining = waitTime - timeElapsed;
    }
}

}  // namespace TimeUtils
