#include "router.h"
#include <cstdio>


unsigned int Router::getId(int ip) {
	map<unsigned int, unsigned int>::iterator it = id.find(ip);
	if (it != id.end())
		return it->second;
	printf("creating new id\n");
}

void Router::sendPacket(Packet p) {

}

void Router::addLink(Link l) {
	getId(l.dest);
}
