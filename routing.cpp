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
map<unsigned int, int> ids;


priority_queue<Packet*, vector<Packet*>, PacketComperator> packets;

unsigned long long now() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (long)ts.tv_sec * (long)1000000000 + (long)ts.tv_nsec;
}

void addPacket(int delay, Packet *packet) {
	packet->time = now() + (long)delay * 1000000;
	packets.push(packet);
}

unsigned int getIp(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	return a << 24 | b << 16 | c << 8 | d;
}

//char* getIpString(unsigned int ip) {
	//printf("192.168.0.%d", ip);
//}

void initRouters(int routersCount, int routerLinks) {
	vector<Link> links;
	routers.resize(routersCount);
	for (int i = 0; i < routers.size(); ++i) {
		routers[i].ip = getIp(192, 168, 0, i+1);
		routers[i].me = routers[i].getId(routers[i].ip);
		ids[routers[i].ip] = i;
	}

	for (int i = 1; i < routers.size(); ++i)
		for (int j = 0; j < routerLinks && j < i; ++j) {
			int to = rand() % i;
			//printf("%d -> %d\n", i, to);
			int w = rand () % 1000 + 100;
			routers[i].addLink(routers[to].ip, w);
			routers[to].addLink(routers[i].ip, w);
		}

	for (int i = 0; i < routers.size(); ++i)
		for (int j = 0; j < routers[i].links[routers[i].me].size(); ++j) {
			Link *l = &routers[i].links[routers[i].me][j];
			routers[i].broadcastLink(routers[i].ip, l->dest, l->weight);
			//printf("%u %u\n", routers[i].ip & 255, to & 255);
			//printf("%d %d\n", i, l->dest);
		}
}

void sendPacket(int from, int to, int data) {
	DataPacket *packet = new DataPacket();
	packet->source = packet->from = routers[from].ip;
	packet->dest = routers[to].ip;
	packet->data = data;
	printf("SEND PACKET %d -> %d\n", packet->source & 255, packet->dest & 255);
	routers[from].sendDestinationPacket(packet);
}

void simulatePackets() {
	while (!packets.empty()) {
		Packet *p = packets.top();
		packets.pop();
		unsigned long long n = now();
		//printf("check    %llu\n", now());
		//printf("wait for %llu\n", p->time);
		if (n < p->time) {
			//printf("sleep for %lus", p.time - n);
			fflush(stdout);
			usleep((p->time - n) / 1000);
		}
		while (now() < p->time);
		int to = ids[p->to];
		printf("-----------------------------------------\n");
		printf("Router 192.168.0.%d received packet.\n", p->to & 255);
		routers[to].receivePacket(p);
		printf("-----------------------------------------\n");
	}
}

int main() {
	//initRouters(2, 1);
	initRouters(5, 2);

	simulatePackets();

	printf("\n");
	printf("\n");
	printf("\n");
	printf("Network initialized.\n");

	for (int i = 0; i < 5; ++i) {
		//int from = random() % routers.size()
		sendPacket(random() % routers.size(), random() % routers.size(), 101+i);
	}

	simulatePackets();
	return 0;
}

