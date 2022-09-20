#include "Socket.h"

#include <stdexcept>
#include <string>

namespace NetUtils {

////////////////////////////////////////////////////////////////////////////////
// Windows
////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32

bool init()
{
    // Initialize Winsock
    WSADATA wsaData;
    int lastResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    return lastResult == 0;
}

////////////////////////////////////////////////////////////////////////////////
// Fallback implementation
////////////////////////////////////////////////////////////////////////////////

#else

bool init()
{
    return false;
}

#endif

}  // namespace NetUtils

////////////////////////////////////////////////////////////////////////////////
// Windows
////////////////////////////////////////////////////////////////////////////////

DatagramSocket::DatagramSocket(int port)
{
    // Define hints to create a UDP listen socket
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE;

    // Get the localhost address for the desired port
    addrinfo* result = nullptr;
    int lastResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
    if (lastResult != 0)
    {
        WSACleanup();
        throw std::runtime_error("Error getting host address: " + std::to_string(lastResult));
    }

    // Create the socket
    SOCKET ListenSocket = INVALID_SOCKET;
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        WSACleanup();
        throw std::runtime_error("Error creating socket: " + std::to_string(WSAGetLastError()));
    }

    // Setup the TCP listening socket
    lastResult = bind(ListenSocket, result->ai_addr, (int) result->ai_addrlen);
    if (lastResult == SOCKET_ERROR)
    {
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        throw std::runtime_error("Error binding socket: " + std::to_string(WSAGetLastError()));
    }

    // At this point we no longer need the address info
    freeaddrinfo(result);

    // Socket is now ready to receive packets
}
