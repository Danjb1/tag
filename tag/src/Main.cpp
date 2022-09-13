#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Application.h"
#include "Shaders.h"
#include "TimeUtils.h"

static constexpr int windowWidth = 800;
static constexpr int windowHeight = 600;
static const std::string versionString = "1.0.0";
static const std::string windowTitle = "Tag v" + versionString;
static constexpr bool vsync = true;

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS)
    {
        app->keyPressed(key, mods);
    }
}

static void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->windowResized();
}

int main(void)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Prepare for window creation
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    // Center the window in the primary monitor
    int monitorCount;
    int monitorX;
    int monitorY;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);
    glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);
    glfwSetWindowPos(
            window, monitorX + (videoMode->width - windowWidth) / 2, monitorY + (videoMode->height - windowHeight) / 2);

    // Make the window visible
    glfwShowWindow(window);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Register GLFW callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    // Initialize GLEW
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewStatus) << "\n";
        return -1;
    }

    // Initialize shaders
    Shaders::initializeShaders();

    // Enable vsync
    glfwSwapInterval(vsync ? 1 : 0);

    // Create the application and store a pointer to it in GLFW
    Application app(window);
    glfwSetWindowUserPointer(window, &app);

    // Start the application
    double lastUpdateTime = glfwGetTime();
    while (app.isRunning())
    {
        // Measure time
        double nowTime = glfwGetTime();
        float deltaTime = static_cast<float>(nowTime - lastUpdateTime);

        // Is an update due?
        if (deltaTime > TimeUtils::frameTime)
        {
            lastUpdateTime = nowTime;
            int numUpdatesPerformed = 0;

            // Process GLFW event queue
            glfwPollEvents();

            // Update according to our desired FPS.
            // This may update more than once if we are falling behind.
            while (deltaTime > TimeUtils::frameTime && numUpdatesPerformed < TimeUtils::maxUpdatesPerRender)
            {
                app.tick();
                deltaTime -= TimeUtils::frameTime;
                ++numUpdatesPerformed;
            }

            // Render
            app.render();

            // If vsync is enabled, this blocks until the next screen refresh
            glfwSwapBuffers(window);
        }
        else
        {
            // Wait until the next update is due
            float timeUntilNextTick = TimeUtils::frameTime - deltaTime;
            TimeUtils::wait(timeUntilNextTick);
        }
    }

    // Exit cleanly
    glfwTerminate();

    return 0;
}
