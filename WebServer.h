/**
 * @file WebServer.h
 * @brief Defines the WebServer class for processing requests.
 */

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @brief Simulates a web server that processes one request at a time.
 *
 * Each server has a unique ID and tracks its busy state.
 * When assigned a request, it processes it for the request's
 * specified number of clock cycles.
 */
class WebServer {
private:
    int id;                    ///< Unique server identifier.
    bool busy;                 ///< Whether the server is currently processing.
    Request currentRequest;    ///< The request currently being processed.
    int remainingTime;         ///< Clock cycles remaining for the current request.

public:
    /**
     * @brief Constructs a WebServer with the given ID.
     * @param id Unique identifier for the server.
     */
    WebServer(int id);

    /**
     * @brief Assigns a request to this server for processing.
     * @param req The Request to process.
     */
    void assignRequest(Request req);

    /**
     * @brief Advances the server by one clock cycle.
     *
     * Decrements remaining time. When time reaches zero,
     * the server becomes free.
     */
    void tick();

    /**
     * @brief Checks if the server is currently busy.
     * @return True if the server is processing a request.
     */
    bool isBusy() const;

    /**
     * @brief Returns the server's unique ID.
     * @return The server ID.
     */
    int getId() const;

    /**
     * @brief Returns the request currently being processed.
     * @return The current Request.
     */
    Request getCurrentRequest() const;
};

#endif
