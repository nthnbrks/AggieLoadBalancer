/**
 * @file RequestQueue.h
 * @brief Defines the RequestQueue class for managing a queue of requests.
 */

#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include "Request.h"
#include <queue>

/**
 * @brief A queue wrapper for managing Request objects.
 *
 * Provides enqueue, dequeue, and status checking operations
 * for the load balancer's request queue.
 */
class RequestQueue {
private:
    std::queue<Request> queue; ///< Internal queue storing Request objects.

public:
    /**
     * @brief Adds a request to the back of the queue.
     * @param req The Request to enqueue.
     */
    void enqueue(Request req);

    /**
     * @brief Removes and returns the request at the front of the queue.
     * @return The front Request.
     */
    Request dequeue();

    /**
     * @brief Checks if the queue is empty.
     * @return True if the queue has no requests.
     */
    bool isEmpty() const;

    /**
     * @brief Returns the number of requests in the queue.
     * @return The current queue size.
     */
    int getSize() const;
};

#endif
