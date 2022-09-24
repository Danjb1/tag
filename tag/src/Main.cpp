#define ENET_IMPLEMENTATION
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <enet.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <iostream>

#include "Application.h"
#include "ProgramOptions.h"
#include "Shaders.h"
#include "World.h"

static constexpr int windowWidth = 800;
static constexpr int windowHeight = 600;
static const std::string versionString = "1.0.0";
static const std::string defaultWindowTitle = "Tag v" + versionString;

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
    if (options.isNetworked() && enet_initialize() != 0)
    {
        std::cerr << "Failed to initialize networking library\n";
        return -1;
    }

    // Initialize network connection
    ENetHost* connection = nullptr;
    if (options.isHost())
    {
        // Find the localhost address on the desired port
        ENetAddress address = { 0 };
        address.host = ENET_HOST_ANY;
        address.port = options.getPort();

        // Create server
        int maxClients = options.getNumPlayers() - 1;
        connection = enet_host_create(
                &address,
                maxClients,  // Allow 1 outgoing connection per client
                2,           // Allow up to 2 channels to be used, 0 and 1
                0,           // Assume any amount of incoming bandwidth
                0            // Assume any amount of outgoing bandwidth
        );
        if (!connection)
        {
            std::cerr << "Failed to create server\n";
            return -1;
        }

        std::cout << "Listening on port: " << address.port << "\n";
    }
    else if (options.isClient())
    {
        // Create the client connection
        connection = enet_host_create(
                nullptr,  // Create a client
                1,        // Only allow 1 outgoing connection
                2,        // Allow up 2 channels to be used, 0 and 1
                0,        // Assume any amount of incoming bandwidth
                0         // Assume any amount of outgoing bandwidth
        );
        if (!connection)
        {
            std::cerr << "Failed to create net client\n";
            return -1;
        }

        // Connect to the server
        ENetAddress address = { 0 };
        ENetPeer* peer;
        enet_address_set_host(&address, options.getHostAddress().c_str());
        address.port = options.getPort();

        // Attempt the connection, allocating the two channels 0 and 1
        peer = enet_host_connect(connection, &address, 2, 0);
        if (!peer)
        {
            std::cerr << "Failed to initiate client connection\n";
            return -1;
        }

        std::cout << "Connecting to " << options.getHostAddress() << ":" << address.port << "\n";
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
    std::string windowTitle = defaultWindowTitle;
    if (options.isHost())
    {
        windowTitle += " [host]";
    }
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
    Application app(window, options.getNumPlayers(), connection);
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
    enet_host_destroy(connection);
    enet_deinitialize();

    return 0;
}
