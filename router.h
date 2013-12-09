#ifndef ROUTER_H
#define ROUTER_H

#include <map>
#include <vector>

#include "packet.h"

using namespace std;


class Router {

	map<unsigned int, unsigned int> id;

	bool graphChanged;

	vector<int> next;
	int getNext(int dest);
	void calcPaths();

	Link* getLink(unsigned int from, unsigned int to);
public:
	Router();
	int me;
	unsigned int ip;

	vector<int> ips;
	vector<vector<Link> > links;
	unsigned int getId(int ip);

	void addLink(unsigned int to, int weight);
	void updateLink(unsigned int from, Link nl);
	void receivePacket(Packet *p);
	void sendDestinationPacket(DestinationPacket *p);
	void sendPacket(Packet *p);
	
	void broadcastLink(unsigned int from, unsigned int to, int weight);
	bool insertLink(unsigned int from, Link link);
};

#endif
