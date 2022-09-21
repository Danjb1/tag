# To Do

## WIP

- Basic networking
    - Create a script to automatically start a host and client
    - Disconnect clients when exiting the application
    - Call enet_host_service at the start of the game loop
        - See: http://enet.bespin.org/Tutorial.html
    - Game should not start until all players have joined
    - Host should accept client connections and assign them player numbers
    - Prevent moving other players
    - Prevent tagging on clients
    - Clients should send their position at the end of the tick
    - Host should listen for client messages and move clients to their reported positions
    - Host should send the game state to all clients at the end of the tick

## Features

- VFX
    - Tagged player pulse
    - Winner celebration
    - Border flash / shake when tagging
    - Player trails
- Inverted tag mode (time goes down when tagged)
- Online play
- Power-ups
- Gamepad support
- AI
- Steam support
- Configurable controls
- Icon for window
- Icon for executable

## Tech Debt

- Tidy up program initialization
- Create a window class to hold the window properties, etc.
- Cross-platform support (TimeUtils)
    - Consider moving OS-specific logic into separate files
