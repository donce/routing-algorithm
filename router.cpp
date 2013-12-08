#include "router.h"
#include "routing.h"
#include <cstdio>
#include <cstdlib>

Router::Router() {
	graphChanged = true;
}

unsigned int Router::getId(int ip) {
	map<unsigned int, unsigned int>::iterator it = id.find(ip);
	if (it != id.end())
		return it->second;
	links.push_back(vector<Link>());
	printf("creating new id\n");
}

void Router::calcPaths() {
	printf("calculating paths\n");
	int n = links.size();

	printf("n %d\n", n);
	int *dist = new int[n];
	bool *added = new bool[n];
	int *back = new int[n];
	
	for (int i = 0; i < n; ++i) {
		dist[i] = -1;
		added[i] = false;
		back[i] = -1;
	}

	dist[0] = 0;
	
	int now = 0;

	while (now != -1) {
		printf("in %d\n", now);
		added[now] = true;

		for (int i = 0; i < links[now].size(); ++i) {
			int newDist = dist[now] + links[now][i].weight;
			int to = links[now][i].dest;
			if (dist[to] == -1 || newDist < dist[to]) {
				dist[to] = newDist;
				back[to] = now;
			}
		}
		//find new
		now = -1;
		for (int i = 0; i < n; ++i)
			if (!added[i] && dist[i] >= 0 && (now == -1 || dist[i] < dist[now]))
				now = i;
	}

	delete dist;
	delete back;
}

int Router::getNext(int dest) {
	if (graphChanged) {
		calcPaths();
		graphChanged = false;
	}
	return -1;
}

void Router::receivePacket(Packet *p) {
	if (p->isDestination()) {
		DestinationPacket *dp = (DestinationPacket*)&p;
		if (dp->dest == ip) {
			printf("Packet received it's destination\n");
		}
		else {
			int to = getNext(dp->dest);
			if (to == -1) {
				printf("no known path\n");
				return;
			}
			sendPacket(dp);
		}
	}
	else if (p->isBroadcast()) {

	}
	else
		printf("unknown packet type\n");

	printf("Packet received\n");
}

void Router::sendPacket(Packet *p) {
	printf("router.sendPacket b %d d %d\n", p->isBroadcast(), p->isDestination());
	addPacket(rand() % 4000 + 1000, p);
}

void Router::addLink(Link l) {
	int id = getId(l.dest);

}
