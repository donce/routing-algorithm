#ifndef PACKET_H
#define PACKET_H

struct Link {
	unsigned int dest;
	unsigned int weight;
};

class Packet {
public:
	int from, to;
	int source, dest;
	int time;
	int type;
	bool operator<(const Packet &p) {
		return time < p.time;
	}
};

class PacketComperator {
public:
	bool operator()(const Packet &a, const Packet &b) {
		return a.time < b.time;
	}
};

class RouterUpdatePacket {
public:
	int router;
};

class LinkUpdatePacket {
public:
	int linkFrom;
	Link link;
};

class DataPacket : public Packet {
public:
	int data;
};

#endif
