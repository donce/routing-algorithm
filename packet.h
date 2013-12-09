#ifndef PACKET_H
#define PACKET_H

struct Link {
	unsigned int dest;
	unsigned int weight;
};

class Packet {
public:
	unsigned int from, to;
	unsigned long long time;
	int type;
	bool operator<(const Packet &p) {
		return time < p.time;
	}
	virtual bool isDestination() {
		return false;
	}
	virtual bool isBroadcast() {
		return false;
	}
};

class DestinationPacket : public Packet {
public:
	unsigned int source, dest;
	virtual bool isDestination() {
		return true;
	}
};

class BroadcastPacket : public Packet {
public:
	virtual bool isBroadcast() {
		return true;
	}
};

class PacketComperator {
public:
	bool operator()(const Packet* a, const Packet* b) {
		return a->time > b->time;
	}
};

//class BroadcastPacket : Packet {

//};

//class RouterUpdatePacket : public BroadcastPacket {
//public:
	//int router;
//};

class LinkUpdatePacket : public BroadcastPacket {
public:
	int linkFrom;
	Link link;
};

class DataPacket : public DestinationPacket {
public:
	int data;
};

#endif
