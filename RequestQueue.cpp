/**
 * @file RequestQueue.cpp
 * @brief Implements the RequestQueue class.
 */

#include "RequestQueue.h"

/**
 * @brief Adds a request to the back of the queue.
 * @param req The Request to add.
 */
void RequestQueue::enqueue(Request req) {
    queue.push(req);
}

/**
 * @brief Removes and returns the front request.
 * @return The Request at the front of the queue.
 */
Request RequestQueue::dequeue() {
    Request front = queue.front();
    queue.pop();
    return front;
}

/**
 * @brief Checks if the queue is empty.
 * @return True if empty, false otherwise.
 */
bool RequestQueue::isEmpty() const {
    return queue.empty();
}

/**
 * @brief Returns the current size of the queue.
 * @return Number of requests in the queue.
 */
int RequestQueue::getSize() const {
    return queue.size();
}
