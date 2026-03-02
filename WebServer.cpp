/**
 * @file WebServer.cpp
 * @brief Implements the WebServer class.
 */

#include "WebServer.h"

/**
 * @brief Constructs a WebServer with the given ID, initially idle.
 * @param id Unique identifier for the server.
 */
WebServer::WebServer(int id) {
    this->id = id;
    busy = false;
    remainingTime = 0;
}

/**
 * @brief Assigns a request to this server.
 *
 * Sets the server to busy and initializes the remaining
 * time to the request's processing time.
 *
 * @param req The Request to process.
 */
void WebServer::assignRequest(Request req) {
    currentRequest = req;
    busy = true;
    remainingTime = req.time;
}

/**
 * @brief Advances the server by one clock cycle.
 *
 * If the server is busy, decrements the remaining time.
 * When remaining time reaches zero, the server becomes free.
 */
void WebServer::tick() {
    if (busy) {
        remainingTime--;
        if (remainingTime <= 0) {
            busy = false;
        }
    }
}

/**
 * @brief Checks if the server is currently processing a request.
 * @return True if busy, false if idle.
 */
bool WebServer::isBusy() const {
    return busy;
}

/**
 * @brief Returns the server's unique ID.
 * @return The server ID.
 */
int WebServer::getId() const {
    return id;
}

/**
 * @brief Returns the request currently being processed.
 * @return The current Request.
 */
Request WebServer::getCurrentRequest() const {
    return currentRequest;
}
