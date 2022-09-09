#pragma once

namespace TimeUtils {

/** Desired frames per second for the game logic. */
static constexpr int fps = 60;

/** Time represented by each frame, in seconds. */
static constexpr float frameTime = 1.f / fps;

/** Minimum time to wait before we will consider sleeping, in milliseconds. */
static constexpr int minSleepTime = 10;

/**
 * Maximum number of updates that we will perform between renders, in the case of lag.
 *
 * After this, the game will appear to slow down.
 */
static constexpr int maxUpdatesPerRender = 5;

}  // namespace TimeUtils
