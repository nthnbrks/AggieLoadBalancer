CC = g++
CFLAGS = -Wall -Werror

all: loadbalancer

loadbalancer: main.o Request.o RequestQueue.o WebServer.o LoadBalancer.o Switch.o
	$(CC) $(CFLAGS) -o loadbalancer main.o Request.o RequestQueue.o WebServer.o LoadBalancer.o Switch.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Request.o: Request.cpp
	$(CC) $(CFLAGS) -c Request.cpp
	
RequestQueue.o: RequestQueue.cpp
	$(CC) $(CFLAGS) -c RequestQueue.cpp

WebServer.o: WebServer.cpp
	$(CC) $(CFLAGS) -c WebServer.cpp

LoadBalancer.o: LoadBalancer.cpp
	$(CC) $(CFLAGS) -c LoadBalancer.cpp

Switch.o: Switch.cpp
	$(CC) $(CFLAGS) -c Switch.cpp

clean:
	rm -f loadbalancer *.o
