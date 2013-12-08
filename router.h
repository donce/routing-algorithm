#ifndef ROUTER_H
#define ROUTER_H

#include <map>
#include <vector>

#include "packet.h"

using namespace std;


struct Router {
	vector<vector<Link> > links;
	map<unsigned int, unsigned int> id;

	bool graphChanged;

	unsigned int getId(int ip);
	int getNext(int dest);
	void calcPaths();
public:
	Router();
	unsigned int ip;

	void addLink(Link l);
	void receivePacket(Packet *p);
	void sendPacket(Packet *p);
};

#endif
