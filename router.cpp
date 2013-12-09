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
	//printf("creating new id for %d\n", ip & 255);
	links.push_back(vector<Link>());
	ips.push_back(ip);
	next.push_back(-1);
	id[ip] = links.size()-1;
	return links.size()-1;
}

void Router::calcPaths() {
	printf("Router %d: calculating paths\n", ip & 255);
	int n = links.size();

	//printf("n %d\n", n);
	int *dist = new int[n];
	bool *added = new bool[n];
	int *back = new int[n];
	
	for (int i = 0; i < n; ++i) {
		dist[i] = -1;
		added[i] = false;
		back[i] = -1;
	}

	dist[me] = 0;
	int now = me;

	while (now != -1) {
		added[now] = true;

		for (int i = 0; i < links[now].size(); ++i) {
			int newDist = dist[now] + links[now][i].weight;
			int to = getId(links[now][i].dest);
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
	
	//backtrace
	for (int i = 0; i < n; ++i) {
		int now = i;
		if (now == me) {
			//me
			next[i] = -1;
		}
		else {
			//backtrace
			while (back[now] != me) {
				now = back[now];
			}
			next[i] = now;
			//printf("result %d\n", next[i]);
		}
	}

	delete dist;
	delete back;
}

int Router::getNext(int dest) {
	if (graphChanged) {
		calcPaths();
		graphChanged = false;
	}
	int to = getId(dest);
	//printf("get me %d\n", ip & 255);
	return next[to];
}

void Router::sendDestinationPacket(DestinationPacket *dp) {
	int to = getNext(dp->dest);
	if (to == -1) {
		printf("no known path\n");
		return;
	}
	//printf("me %d to %d next %d\n", ip & 255, dp->dest & 255, ips[to] & 255);
	dp->from = ip;
	dp->to = ips[to];
	sendPacket(dp);
}

void Router::receivePacket(Packet *p) {
	//printf("receive\n");
	if (p->isDestination()) {
		DataPacket *dp = (DataPacket*)p;
        //printf("dest %d me %d\n", dp->dest & 255, ip & 255);
		if (dp->dest == ip) {
			printf("Packet received it's destination\n");
			printf("Data: %d\n", dp->data);
			//TODO: destroy
		}
		else {
			sendDestinationPacket(dp);
		}
	}
	else if (p->isBroadcast()) {
		LinkUpdatePacket *bp = (LinkUpdatePacket*)p;
		//parse
		updateLink(bp->linkFrom, bp->link);
	}
	else
		printf("unknown packet type\n");

	//printf("Packet received %d\n", ip & 255);
}

void Router::sendPacket(Packet *p) {
	//printf("sending at %d\n", ip & 255);
	Link *l = getLink(ip, p->to);
	addPacket(l->weight, p);
	//addPacket(rand() % 4000 + 1000, p);
}


void Router::addLink(unsigned int to, int weight) {
	int id = getId(to);
	//printf("id %u\n", id);
	Link *l = getLink(ip, to);
	if (l == NULL) {
		//printf("create new\n");
		Link nl;
		nl.dest = to;
		nl.weight = weight;
		links[me].push_back(nl);
	}
	else {
		l->weight = weight;
		//l->
		//printf("already is\n");

	}
}

void Router::updateLink(unsigned int from, Link newl) {
	LinkUpdatePacket *p = new LinkUpdatePacket();
	//addPacket(100, p);
	//Link *oldl = NULL;
	Link *oldl = getLink(from, newl.dest);
	//for (int i = 0; i < links[me].size(); ++i)
		//if (links[me][i].dest == newl.dest) {
			//oldl = &links[me][i];
		//}

	if (oldl == NULL) {
		//new
		//nl.dest = to;
		//nl.weight = weight;
		links[getId(from)].push_back(newl);
		printf("Adding new link.\n");

		
	}
	else {
		//update?
		if (oldl->weight == newl.weight) {
			printf("Old value\n");
			return;
		}
		printf("Updating link value.\n");

	}

	graphChanged = true;
	//resend
	broadcastLink(from, newl.dest, newl.weight);
}

Link* Router::getLink(unsigned int from, unsigned int to) {
	int f = getId(from);
	for (int i = 0; i < links[f].size(); ++i)
		if (links[f][i].dest == to)
			return &links[f][i];
	return NULL;
}

void Router::broadcastLink(unsigned int from, unsigned int to, int weight) {
	printf("broadcasting %d->%d %d\n", from & 255, to & 255, weight);

	Link l;
	l.dest = to;
	l.weight = weight;

	LinkUpdatePacket *p = new LinkUpdatePacket();
	p->linkFrom = from;
	p->link = l;

	for (int i = 0; i < links[me].size(); ++i)
		if (i != me) {
			p->to = links[me][i].dest;
			sendPacket(p);
		}
}
