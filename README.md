# AggieLoadBalancer

## Usage

run `make clean & make` to build the AggieLoadBalancer

run `loadbalancer.exe` to run the program

## Configuration

edit `config.txt` to change system parameters:

* servers determines the amount of servers to initialize the balancer with
* runtime determines the amount of clock cycles the balancer should run for
* cooldown determines how long after adding/removing a server the balancer should wait before attempting a similar operation
* arrival determiens how many clock cycles the system waits between generating requests
* blocked requires 2 IP addresses and blocks all IPs within that range. Multiple blocked lines can be added to block multiple separate ranges of IP addresses.

refer to https://people.tamu.edu/~nthnbrks/CS412/AggieLoadBalancer/ for all further documentation
