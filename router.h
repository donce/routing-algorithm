#ifndef ROUTER_H
#define ROUTER_H

#include <map>
#include <vector>

#include "packet.h"

using namespace std;


struct Router {
	unsigned int getId(int ip);
public:
	unsigned int ip;

	map<unsigned int, unsigned int> id;
	vector<vector<Link> > links;

	void addLink(Link l);
	void sendPacket(Packet p);
};


#endif
