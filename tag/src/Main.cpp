#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Application.h"
#include "Shaders.h"
#include "World.h"

static constexpr int windowWidth = 800;
static constexpr int windowHeight = 600;
static const std::string versionString = "1.0.0";
static const std::string windowTitle = "Tag v" + versionString;
static bool fullscreenEnabled = false;
static bool vsyncEnabled = true;
static int numPlayers = 2;

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
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-novsync")
        {
            vsyncEnabled = false;
        }
        else if (arg == "-fullscreen")
        {
            fullscreenEnabled = true;
        }
        else if (arg == "-numPlayers")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "No value supplied for numPlayers\n";
                std::cerr << "Expected: -numPlayers [n]\n";
                return -1;
            }
            try
            {
                numPlayers = std::stoi(argv[i + 1]);
                if (numPlayers < World::minPlayers || numPlayers > World::maxPlayers)
                {
                    throw std::out_of_range("numPlayers out of range");
                }
            }
            catch (const std::invalid_argument&)
            {
                std::cerr << "Invalid value supplied for numPlayers\n";
                return -1;
            }
            catch (const std::out_of_range&)
            {
                std::cerr << "numPlayers must be between " << World::minPlayers << " and " << World::maxPlayers << "\n";
                return -1;
            }
            ++i;  // Skip next argument
        }
        else
        {
            std::cerr << "Invalid argument: " << arg << "\n";
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
    glfwSwapInterval(vsyncEnabled ? 1 : 0);

    // Create the application and store a pointer to it in GLFW
    Application app(window, numPlayers);
    glfwSetWindowUserPointer(window, &app);

    // Make the window visible
    glfwShowWindow(window);

    // Toggle fullscreen immediately if requested
    if (fullscreenEnabled)
    {
        app.toggleFullscreen();
    }

    // Run the application
    app.run();

    // Exit cleanly
    glfwTerminate();

    return 0;
}
