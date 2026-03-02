/**
 * @file Switch.cpp
 * @brief Implements the Switch class for routing requests by job type.
 */

#include "Switch.h"
#include <iostream>
#include <cstdlib>

/**
 * @brief Constructs a Switch with separate LoadBalancers for each job type.
 *
 * Opens a shared log file and passes it to both LoadBalancers.
 *
 * @param procServers Number of initial processing servers.
 * @param streamServers Number of initial streaming servers.
 * @param cooldown Cooldown cycles between scaling actions.
 * @param arrivalRate Average cycles between new random requests.
 */
Switch::Switch(int procServers, int streamServers, int cooldown, int arrivalRate) {
    this->arrivalRate = arrivalRate;
    logFile.open("loadbalancer.log");
    processingLB = new LoadBalancer(procServers, cooldown, "Processing", &logFile);
    streamingLB = new LoadBalancer(streamServers, cooldown, "Streaming", &logFile);
}

/**
 * @brief Destructor. Frees both LoadBalancers and closes the shared log.
 */
Switch::~Switch() {
    delete processingLB;
    delete streamingLB;
    if (logFile.is_open()) {
        logFile.close();
    }
}

/**
 * @brief Routes a request based on its job type.
 *
 * Processing requests (jobType == true) go to the processing LoadBalancer.
 * Streaming requests (jobType == false) go to the streaming LoadBalancer.
 *
 * @param req The Request to route.
 */
void Switch::routeRequest(Request req) {
    if (req.jobType) {
        processingLB->addRequest(req);
    } else {
        streamingLB->addRequest(req);
    }
}

/**
 * @brief Runs the simulation for the specified number of clock cycles.
 *
 * Each cycle steps both LoadBalancers and has a chance to generate
 * a new random request based on the arrival rate. Prints summaries
 * for both LoadBalancers at the end.
 *
 * @param totalTime Total clock cycles to simulate.
 */
void Switch::run(int totalTime) {
    for (int cycle = 1; cycle <= totalTime; cycle++) {
        processingLB->step(cycle);
        streamingLB->step(cycle);

        if (rand() % arrivalRate == 0) {
            routeRequest(generateRandomRequest());
        }
    }

    processingLB->printSummary();
    streamingLB->printSummary();
}

/**
 * @brief Adds a blocked IP range to both LoadBalancers.
 * @param start Start IP of the range.
 * @param end End IP of the range.
 */
void Switch::addBlockedRange(std::string start, std::string end) {
    processingLB->addBlockedRange(start, end);
    streamingLB->addBlockedRange(start, end);
}
