/**
 * @file LoadBalancer.cpp
 * @brief Implements the LoadBalancer class.
 */

#include "LoadBalancer.h"
#include <iostream>
#include <cstdlib>
#include <sstream>

/**
 * @brief Converts an IP address string to a numeric value for comparison.
 * @param ip The IP address string in "x.x.x.x" format.
 * @return The IP address as an unsigned long.
 */
static unsigned long ipToNum(std::string ip) {
    unsigned long result = 0;
    std::istringstream iss(ip);
    std::string octet;
    for (int i = 0; i < 4; i++) {
        std::getline(iss, octet, '.');
        result = (result << 8) + std::stoul(octet);
    }
    return result;
}

/**
 * @brief Constructs a LoadBalancer with the specified number of servers.
 * @param numServers Initial number of web servers.
 * @param cooldownTime Cycles to wait between scaling actions.
 * @param name Name for identification in logs.
 * @param log Optional shared log file. Creates its own if null.
 */
LoadBalancer::LoadBalancer(int numServers, int cooldownTime, std::string name, std::ofstream* log) {
    this->name = name;
    this->cooldownTime = cooldownTime;
    this->cooldown = 0;
    this->clockCycle = 0;
    this->nextServerId = 1;
    this->totalProcessed = 0;
    this->totalBlocked = 0;
    this->serversAdded = 0;
    this->serversRemoved = 0;

    for (int i = 0; i < numServers; i++) {
        servers.push_back(new WebServer(nextServerId++));
    }

    if (log) {
        logFile = log;
        ownsLog = false;
    } else {
        logFile = new std::ofstream("loadbalancer.log");
        ownsLog = true;
    }
}

/**
 * @brief Destructor. Frees all dynamically allocated servers and closes log if owned.
 */
LoadBalancer::~LoadBalancer() {
    for (size_t i = 0; i < servers.size(); i++) {
        delete servers[i];
    }
    if (ownsLog && logFile) {
        logFile->close();
        delete logFile;
    }
}

/**
 * @brief Runs the full simulation loop for standalone operation.
 *
 * Each cycle: ticks servers, randomly adds new requests,
 * checks scaling thresholds, and logs status every 100 cycles.
 *
 * @param totalTime Number of clock cycles to simulate.
 */
void LoadBalancer::run(int totalTime) {
    for (clockCycle = 1; clockCycle <= totalTime; clockCycle++) {
        tick();

        if (rand() % 5 == 0) {
            addRequest(generateRandomRequest());
        }

        balanceServers();

        if (clockCycle % 100 == 0) {
            logStatus();
        }
    }
    printSummary();
}

/**
 * @brief Executes a single cycle for use by the Switch class.
 * @param cycle The current clock cycle number.
 */
void LoadBalancer::step(int cycle) {
    clockCycle = cycle;
    tick();
    balanceServers();
    if (clockCycle % 100 == 0) {
        logStatus();
    }
}

/**
 * @brief Processes one clock cycle.
 *
 * Assigns queued requests to any free servers, then ticks
 * all servers. Increments totalProcessed when a server finishes.
 */
void LoadBalancer::tick() {
    for (size_t i = 0; i < servers.size(); i++) {
        if (!servers[i]->isBusy() && !requestQueue.isEmpty()) {
            servers[i]->assignRequest(requestQueue.dequeue());
        }
    }

    for (size_t i = 0; i < servers.size(); i++) {
        if (servers[i]->isBusy()) {
            servers[i]->tick();
            if (!servers[i]->isBusy()) {
                totalProcessed++;
            }
        }
    }
}

/**
 * @brief Checks queue size against thresholds and scales servers.
 *
 * If queue > 80 * numServers, adds a server.
 * If queue < 50 * numServers, removes a server.
 * Respects cooldown period between scaling actions.
 */
void LoadBalancer::balanceServers() {
    if (cooldown > 0) {
        cooldown--;
        return;
    }

    int queueSize = requestQueue.getSize();
    int numServers = servers.size();
    if (numServers == 0) return;

    if (queueSize > 80 * numServers) {
        addServer();
        cooldown = cooldownTime;
    } else if (queueSize < 50 * numServers && numServers > 1) {
        removeServer();
        cooldown = cooldownTime;
    }
}

/**
 * @brief Adds a new server to the pool and logs the event.
 */
void LoadBalancer::addServer() {
    servers.push_back(new WebServer(nextServerId++));
    serversAdded++;
    std::cout << "\033[32m[" << name << "][Cycle " << clockCycle << "] Server ADDED (total: " << servers.size() << ")\033[0m\n";
    *logFile << "[" << name << "][Cycle " << clockCycle << "] Server ADDED (total: " << servers.size() << ")\n";
}

/**
 * @brief Removes an idle server from the pool and logs the event.
 *
 * Prefers removing an idle server. Will not reduce below 1 server.
 */
void LoadBalancer::removeServer() {
    if (servers.size() <= 1) return;

    size_t idx = servers.size() - 1;
    for (size_t i = servers.size() - 1; i > 0; i--) {
        if (!servers[i]->isBusy()) {
            idx = i;
            break;
        }
    }

    delete servers[idx];
    servers.erase(servers.begin() + idx);
    serversRemoved++;
    std::cout << "\033[31m[" << name << "][Cycle " << clockCycle << "] Server REMOVED (total: " << servers.size() << ")\033[0m\n";
    *logFile << "[" << name << "][Cycle " << clockCycle << "] Server REMOVED (total: " << servers.size() << ")\n";
}

/**
 * @brief Adds a request to the queue if not blocked by the firewall.
 * @param req The Request to add.
 */
void LoadBalancer::addRequest(Request req) {
    if (isBlocked(req.ipIn)) {
        totalBlocked++;
        return;
    }
    requestQueue.enqueue(req);
}

/**
 * @brief Adds an IP range to the block list.
 * @param start Start IP of the range to block.
 * @param end End IP of the range to block.
 */
void LoadBalancer::addBlockedRange(std::string start, std::string end) {
    IPRange range;
    range.start = start;
    range.end = end;
    blockedRanges.push_back(range);
}

/**
 * @brief Checks if an IP falls within any blocked range.
 *
 * Converts IPs to numeric values for correct comparison.
 *
 * @param ip The IP address to check.
 * @return True if the IP is in a blocked range.
 */
bool LoadBalancer::isBlocked(std::string ip) {
    unsigned long ipNum = ipToNum(ip);
    for (size_t i = 0; i < blockedRanges.size(); i++) {
        unsigned long startNum = ipToNum(blockedRanges[i].start);
        unsigned long endNum = ipToNum(blockedRanges[i].end);
        if (ipNum >= startNum && ipNum <= endNum) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Logs the current cycle status to console and log file.
 */
void LoadBalancer::logStatus() {
    std::cout << "\033[35m[" << name << "][Cycle " << clockCycle << "] Queue: " << requestQueue.getSize() << " | Servers: " << servers.size() << " | Processed: " << totalProcessed << "\033[0m\n";
    *logFile << "[" << name << "][Cycle " << clockCycle << "] Queue: " << requestQueue.getSize() << " | Servers: " << servers.size() << " | Processed: " << totalProcessed << "\n";
}

/**
 * @brief Prints a summary of the simulation to console and log file.
 */
void LoadBalancer::printSummary() {
    std::cout << "\n\033[36m" << name << " Summary\n"
              << "Cycles: " << clockCycle - 1 << "\n"
              << "Processed: " << totalProcessed << "\n"
              << "Blocked: " << totalBlocked << "\n"
              << "Final queue: " << requestQueue.getSize() << "\n"
              << "Final servers: " << servers.size() << "\n"
              << "Servers added: " << serversAdded << "\n"
              << "Servers removed: " << serversRemoved << "\033[0m\n";

    *logFile << "\n" << name << " Summary\n"
             << "Cycles: " << clockCycle - 1 << "\n"
             << "Processed: " << totalProcessed << "\n"
             << "Blocked: " << totalBlocked << "\n"
             << "Final queue: " << requestQueue.getSize() << "\n"
             << "Final servers: " << servers.size() << "\n"
             << "Servers added: " << serversAdded << "\n"
             << "Servers removed: " << serversRemoved << "\n\n"
             << "Gig 'em.\n";
}

/**
 * @brief Returns the current queue size.
 * @return Number of pending requests.
 */
int LoadBalancer::getQueueSize() const {
    return requestQueue.getSize();
}

/**
 * @brief Returns the current number of active servers.
 * @return Number of servers.
 */
int LoadBalancer::getServerCount() const {
    return servers.size();
}
