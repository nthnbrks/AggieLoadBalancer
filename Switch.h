/**
 * @file Switch.h
 * @brief Defines the Switch class for routing requests by job type.
 */

#ifndef SWITCH_H
#define SWITCH_H

#include "LoadBalancer.h"
#include <fstream>

/**
 * @brief Routes incoming requests to separate LoadBalancers based on job type.
 *
 * Processing requests (jobType == true) go to one LoadBalancer,
 * and streaming requests (jobType == false) go to another.
 * Each LoadBalancer manages its own server pool independently.
 */
class Switch {
private:
    LoadBalancer* processingLB; ///< LoadBalancer for processing jobs.
    LoadBalancer* streamingLB;  ///< LoadBalancer for streaming jobs.
    std::ofstream logFile;      ///< Shared log file for both LoadBalancers.
    int arrivalRate;            ///< Average cycles between new request arrivals.

public:
    /**
     * @brief Constructs a Switch with two LoadBalancers.
     * @param procServers Number of initial servers for processing.
     * @param streamServers Number of initial servers for streaming.
     * @param cooldown Cooldown cycles between scaling actions.
     * @param arrivalRate Average cycles between new random requests.
     */
    Switch(int procServers, int streamServers, int cooldown, int arrivalRate = 50);

    /**
     * @brief Destructor. Frees both LoadBalancers and closes the log file.
     */
    ~Switch();

    /**
     * @brief Routes a request to the appropriate LoadBalancer by job type.
     * @param req The Request to route.
     */
    void routeRequest(Request req);

    /**
     * @brief Runs the simulation for a given number of clock cycles.
     *
     * Steps both LoadBalancers each cycle and randomly generates
     * new requests based on the arrival rate.
     *
     * @param totalTime Total clock cycles to simulate.
     */
    void run(int totalTime);

    /**
     * @brief Adds a blocked IP range to both LoadBalancers.
     * @param start Start of the IP range to block.
     * @param end End of the IP range to block.
     */
    void addBlockedRange(std::string start, std::string end);
};

#endif
