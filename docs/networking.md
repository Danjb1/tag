# Networking Strategy

<!----------------------------------------------------------------------------->
## General
<!----------------------------------------------------------------------------->

- Game is client-authoritative, with some server-side validation.
    - This helps to keep things as responsive as possible.
- Game uses UDP with a mechanism for optional reliable messaging.
    - We should consider using a library for this.
- Network messages should be sent using a net tick rate, not the game tick rate.
- Need to keep track of the frame counter so we can refer to certain frames.
- Suboptimal network conditions can be simulated using clumsy.

<!----------------------------------------------------------------------------->
## Phase 1: Basic Communication
<!----------------------------------------------------------------------------->

After this phase, network games will work but movements will be very jerky and
clients may experience inaccurate tagging.

### Host

- Host a game using `-host -port [port]`
- Assign player numbers to clients as they join
- Can only control player 1
- Start of tick: read messages from clients.
    - Move other players to client-reported positions.
- Run tick as usual.
- End of tick: send updated game state to all clients.

### Client

- Join a game using `-connect [hostAddress]`
- Receive player number from server
- Can only control the assigned player
- Start of tick: read messages from server.
    - Move other players to server-reported positions.
    - Update the tagged player and scores based on the server state.
- Run tick as usual but don't process tags.
- End of tick: send updated position.

<!----------------------------------------------------------------------------->
## Phase 2: Interpolation
<!----------------------------------------------------------------------------->

After this phase, movement will appear smoother.

### Host & Client

- When a player position is received over the network, store it somewhere.
- Interpolate towards these stored positions over time.
    - Somehow we need to only travel in orthogonal lines!

<!----------------------------------------------------------------------------->
## Phase 3: Client-Authoritative Tagging
<!----------------------------------------------------------------------------->

After this phase, tagging will appear more responsive for clients, but sometimes
tags may be allowed that shouldn't be (i.e. if the tagged player dodged).

### Host

- If a client reports a more recent tag, update the tagged player immediately.
- Run tick as usual but only process tags performed by the local player.
- End of tick: send updated game state to all clients.

### Client

- Run tick as usual but only process tags performed by the local player.
- Inform the server if a tag occurred (with the current frame number).

<!----------------------------------------------------------------------------->
## Phase 4: Server Validation
<!----------------------------------------------------------------------------->

After this phase, invalid tags will generally be prevented. Clients may see the
tags take place and then get reverted.

### Host

- Sort client messages by frame number (oldest first).
- For each message received:
    - Rewind the game state based on the specified frame number.
    - Update the client's position for that frame.
    - If a tag was reported:
        - Validate the tag:
            - Was the tagger actually tagged at that time?
            - Were both players close to the tag location?
        - If valid, replay the game from that point.
            - Use the saved positions.
            - Re-evaluate scores.
            - Revalidate any subsequent tags.
- Save the current game state using a circular buffer.

<!----------------------------------------------------------------------------->
## Phase 5: Reliable Messaging
<!----------------------------------------------------------------------------->

After this phase, the game can be reliably started and reset.

### Host

- Game is paused until all clients have connected.
- Start of game: inform clients that game has started.
- Discard messages from clients that are too old.
    - What about messages that are too new (client ahead of the server)?
- End of game: inform clients that game has ended.
- Reset: inform clients that game was restarted.

### Client

- Game is paused until "start game" message is received.
- Discard out-of-order messages that are obsolete.
