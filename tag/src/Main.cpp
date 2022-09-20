////////////////////////////////////////////////////////////////////////////////
// Windows
////////////////////////////////////////////////////////////////////////////////

// <windows.h> needs to be included before GLFW otherwise it causes errors.
// For safety we always include this at the very start of the program.
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif  // _WIN32

////////////////////////////////////////////////////////////////////////////////
// Common
////////////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Application.h"
#include "ProgramOptions.h"
#include "Shaders.h"
#include "Socket.h"
#include "World.h"

static constexpr int windowWidth = 800;
static constexpr int windowHeight = 600;
static const std::string versionString = "1.0.0";
static const std::string windowTitle = "Tag v" + versionString;

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

int main(int argc, char* argv[])
{
    // Parse command-line parameters
    ProgramOptions options(argc, argv);
    if (options.hasError())
    {
        std::cerr << options.getError() << "\n";
        return -1;
    }

    // Initialize networking
    if (options.isNetworked() && !NetUtils::init())
    {
        std::cerr << "Networking not supported by the OS\n";
        return -1;
    }

    // Host
    if (options.isHost())
    {
        try
        {
            // Start listening on an arbitrary port
            DatagramSocket sock(25565);
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << e.what() << "\n ";
            return -1;
        }
    }

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

    // Hide cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Register GLFW callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    // Make the window's context current
    glfwMakeContextCurrent(window);

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
    glfwSwapInterval(options.isVsyncEnabled() ? 1 : 0);

    // Create the application and store a pointer to it in GLFW
    Application app(window, options.getNumPlayers());
    glfwSetWindowUserPointer(window, &app);

    // Make the window visible
    glfwShowWindow(window);

    // Toggle fullscreen immediately if requested
    if (options.isFullscreenEnabled())
    {
        app.toggleFullscreen();
    }

    // Run the application
    app.run();

    // Exit cleanly
    glfwTerminate();

    return 0;
}
