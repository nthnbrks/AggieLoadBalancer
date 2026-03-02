/**
 * @file main.cpp
 * @brief Main driver for the load balancer simulation.
 *
 * Reads configuration from config.txt, creates a Switch with
 * separate processing and streaming LoadBalancers, generates
 * initial requests, and runs the simulation.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <windows.h>
#include "Switch.h"

/**
 * @brief Entry point for the load balancer simulation.
 *
 * Reads config.txt for server count, runtime, cooldown, arrival rate,
 * and blocked IP ranges. Splits servers between processing and streaming,
 * generates an initial queue of servers * 100 requests, then runs
 * the simulation.
 *
 * @return 0 on success.
 */
int main() {
    // Enable ANSI color codes on Windows
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    srand(time(0));

    int numServers = 5;
    int runTime = 1000;
    int cooldown = 10;
    int arrivalRate = 50;
    std::vector<std::pair<std::string, std::string>> blockedRanges;

    std::ifstream configFile("config.txt");
    if (configFile.is_open()) {
        std::string line;
        while (std::getline(configFile, line)) {
            std::istringstream iss(line);
            std::string key;
            iss >> key;

            if (key == "servers") {
                iss >> numServers;
            } else if (key == "runtime") {
                iss >> runTime;
            } else if (key == "cooldown") {
                iss >> cooldown;
            } else if (key == "arrival") {
                iss >> arrivalRate;
            } else if (key == "blocked") {
                std::string start, end;
                iss >> start >> end;
                blockedRanges.push_back({start, end});
            }
        }
        configFile.close();
    }

    int procServers = numServers / 2;
    int streamServers = numServers - procServers;
    Switch sw(procServers, streamServers, cooldown, arrivalRate);

    for (size_t i = 0; i < blockedRanges.size(); i++) {
        sw.addBlockedRange(blockedRanges[i].first, blockedRanges[i].second);
    }

    int initialRequests = numServers * 100;
    std::cout << "Servers: " << procServers << " processing, " << streamServers << " streaming" << " | Runtime: " << runTime << " | Initial requests: " << initialRequests << "\n";
    for (int i = 0; i < initialRequests; i++) {
        sw.routeRequest(generateRandomRequest());
    }

    sw.run(runTime);
    return 0;
}
