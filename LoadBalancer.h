/**
 * @file LoadBalancer.h
 * @brief Defines the LoadBalancer class and IPRange struct.
 */

#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "RequestQueue.h"
#include "WebServer.h"
#include <vector>
#include <string>
#include <fstream>

/**
 * @brief Represents a range of IP addresses for blocking.
 */
struct IPRange {
    std::string start; ///< Start of the blocked IP range.
    std::string end;   ///< End of the blocked IP range.
};

/**
 * @brief Manages a pool of web servers and a request queue.
 *
 * The LoadBalancer distributes incoming requests to available servers,
 * dynamically scales the server pool based on queue thresholds,
 * and blocks requests from specified IP ranges.
 */
class LoadBalancer {
private:
    std::string name;                  ///< Name identifier for logging.
    RequestQueue requestQueue;         ///< Queue of pending requests.
    std::vector<WebServer*> servers;   ///< Pool of web server pointers.
    int clockCycle;                    ///< Current clock cycle.
    int cooldown;                      ///< Remaining cooldown before next scaling action.
    int cooldownTime;                  ///< Cooldown duration between scaling actions.
    int nextServerId;                  ///< Next ID to assign to a new server.
    std::vector<IPRange> blockedRanges;///< List of blocked IP ranges.
    int totalProcessed;                ///< Total requests processed.
    int totalBlocked;                  ///< Total requests blocked by firewall.
    int serversAdded;                  ///< Total servers added during simulation.
    int serversRemoved;                ///< Total servers removed during simulation.
    std::ofstream* logFile;            ///< Pointer to the log file stream.
    bool ownsLog;                      ///< Whether this instance owns the log file.

public:
    /**
     * @brief Constructs a LoadBalancer with initial servers.
     * @param numServers Number of servers to start with.
     * @param cooldownTime Cycles to wait between scaling actions.
     * @param name Name for identification in logs.
     * @param log Optional shared log file stream. If null, creates its own.
     */
    LoadBalancer(int numServers, int cooldownTime, std::string name = "LB", std::ofstream* log = nullptr);

    /**
     * @brief Destructor. Frees servers and closes log if owned.
     */
    ~LoadBalancer();

    /**
     * @brief Runs the simulation for a given number of clock cycles.
     *
     * Used for standalone operation. Handles ticking, random request
     * generation, scaling, and logging.
     *
     * @param totalTime Total clock cycles to simulate.
     */
    void run(int totalTime);

    /**
     * @brief Executes a single clock cycle of the simulation.
     *
     * Used by the Switch class to coordinate multiple LoadBalancers.
     *
     * @param cycle The current clock cycle number.
     */
    void step(int cycle);

    /**
     * @brief Processes one clock cycle: assigns requests and ticks servers.
     */
    void tick();

    /**
     * @brief Checks queue thresholds and adds or removes a server.
     *
     * Adds a server if queue size exceeds 80 * numServers.
     * Removes a server if queue size falls below 50 * numServers.
     */
    void balanceServers();

    /**
     * @brief Dynamically adds a new server to the pool.
     */
    void addServer();

    /**
     * @brief Removes an idle server from the pool.
     */
    void removeServer();

    /**
     * @brief Adds a request to the queue after checking the firewall.
     * @param req The Request to add.
     */
    void addRequest(Request req);

    /**
     * @brief Adds an IP range to the block list.
     * @param start Start of the IP range.
     * @param end End of the IP range.
     */
    void addBlockedRange(std::string start, std::string end);

    /**
     * @brief Checks if an IP address falls within a blocked range.
     * @param ip The IP address to check.
     * @return True if the IP is blocked.
     */
    bool isBlocked(std::string ip);

    /**
     * @brief Logs the current status to console and log file.
     */
    void logStatus();

    /**
     * @brief Prints a summary of the simulation results.
     */
    void printSummary();

    /**
     * @brief Returns the current queue size.
     * @return Number of requests in the queue.
     */
    int getQueueSize() const;

    /**
     * @brief Returns the current number of servers.
     * @return Number of active servers.
     */
    int getServerCount() const;
};

#endif
