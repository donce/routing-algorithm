#include "router.h"
#include "packet.h"

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <queue>

using namespace std;


vector<Router> routers;


priority_queue<Packet, vector<Packet>, PacketComperator> packets;

unsigned int getIp(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	return a << 24 | b << 16 | c << 8 | d;
}


void initRouter(int routersCount, int routerLinks) {
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

void calcRoutes() {
	
}

void sendPacket(int from, int to, int data) {
	DataPacket packet;
	packet.from = from;
	packet.dest = to;
	packet.data = data;
	routers[from].sendPacket(packet);
	//packet.to = to;
}

int main() {
	for (int i = 0; i < 10; ++i) {
		sendPacket(random() % routers.size(), random() % routers.size(), i+1);
	}
	initRouter(5, 2);
	calcRoutes();
	return 0;
}
