#include "router.h"
#include "packet.h"

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <queue>

#include <time.h>
#include <unistd.h>

using namespace std;


vector<Router> routers;


priority_queue<Packet*, vector<Packet*>, PacketComperator> packets;

unsigned long long now() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (long)ts.tv_sec * (long)1000000000 + (long)ts.tv_nsec;
}

void addPacket(int delay, Packet *packet) {
	//printf("b %d d %d\n", packet.isBroadcast(), packet.isDestination());
	packet->time = now();// + (long)delay * 1000000;
	printf("packet added %llu\n", packet->time);
	packets.push(packet);
}

unsigned int getIp(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	return a << 24 | b << 16 | c << 8 | d;
}


void initRouters(int routersCount, int routerLinks) {
	vector<Link> links;
	routers.resize(routersCount);
	for (int i = 0; i < routers.size(); ++i)
		routers[i].ip = getIp(192, 168, 0, i+1);
		
	for (int i = 1; i < routers.size(); ++i)
		for (int j = 0; j < routerLinks && j+1 < i; ++j) {
			int to = rand() % i;
			Link linkTo, linkFrom;
			linkTo.dest = routers[to].ip;
			linkFrom.dest = routers[i].ip;
			linkTo.weight = linkFrom.weight = rand () % 1000;
			routers[i].addLink(linkTo);
			routers[to].addLink(linkFrom);
		}
}

void sendPacket(int from, int to, int data) {
	DataPacket *packet = new DataPacket();
	packet->from = routers[from].ip;
	packet->dest = routers[to].ip;
	packet->data = data;
	routers[from].sendPacket(packet);
}

int main() {
	//DataPacket dp;
	//Packet *packet = &dp;
	//printf("b %d d %d\n", packet.isBroadcast(), packet.isDestination());
	//printf("b %d d %d\n", packet->isBroadcast(), packet->isDestination());
	//printf("%d", packet.isBroadcast());
	//printf("%d", packet.isDestination());
	//return 0;
	//BroadcastPacket bp;
	//printf("%d", bp.isBroadcast());
	//return 0;
	initRouters(5, 2);
	for (int i = 0; i < 1; ++i)
		sendPacket(random() % routers.size(), random() % routers.size(), i+1);
	while (!packets.empty()) {
		Packet *p = packets.top();
		packets.pop();
		unsigned long long n = now();
		printf("check    %llu\n", now());
		printf("wait for %llu\n", p->time);
		if (n < p->time) {
			//printf("sleep for %lus", p.time - n);
			fflush(stdout);
			usleep((p->time - n) / 1000);
		}
		while (now() < p->time);
		printf("%llu\n", now());
		routers[p->to].receivePacket(p);
		delete p;
	}
	return 0;
}
