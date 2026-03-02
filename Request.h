/**
 * @file Request.h
 * @brief Defines the Request struct and request generation functions.
 */

#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @brief Represents a single network request in the load balancer simulation.
 */
struct Request {
    std::string ipIn;  ///< Source IP address of the request.
    std::string ipOut; ///< Destination IP address of the request (stored but not used).
    int time;          ///< Processing time in clock cycles.
    bool jobType;      ///< Job type: true for processing, false for streaming.
};

/**
 * @brief Generates a random Request with random IPs, time, and job type.
 * @return A randomly generated Request.
 */
Request generateRandomRequest();

/**
 * @brief Generates a random IP address in the format "x.x.x.x".
 * @return A string containing a random IP address.
 */
std::string generateRandomIP();

#endif
