/**
 * @file Request.cpp
 * @brief Implements request generation functions.
 */

#include "Request.h"
#include <cstdlib>

/**
 * @brief Generates a random IP address string.
 *
 * Each octet is a random value between 0 and 255.
 *
 * @return A string in the format "x.x.x.x".
 */
std::string generateRandomIP() {
    return std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256) + "." +
           std::to_string(rand() % 256);
}

/**
 * @brief Generates a random Request.
 *
 * Creates a request with random source and destination IPs,
 * a random processing time between 1 and 500 clock cycles,
 * and a random job type (processing or streaming).
 *
 * @return A randomly generated Request.
 */
Request generateRandomRequest() {
    Request req;
    req.ipIn = generateRandomIP();
    req.ipOut = generateRandomIP();
    req.time = (rand() % 500) + 1;
    req.jobType = (rand() % 2 == 0);
    return req;
}
