#include <cstdlib>
#include <cstdio>
#include <map>
#include <vector>

using namespace std;

struct Link {
	unsigned int dest;
	unsigned int weight;
};

struct Router {
	unsigned int ip;
	map<unsigned int, vector<Link> > links;
};

Router router;

unsigned int getIp(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	return a << 24 | b << 16 | c << 8 | d;
}

void initRouter(int routers, int routerLinks) {
	router.ip = getIp(192, 168, 0, 1);
	vector<Link> links;
	for (int i = 2; i < routers+2; ++i) {
		unsigned int ip = getIp(192, 168, 0, i+2);
		for (int j = 0; j < routerLinks; ++j) {
			Link link;
			int to = (rand() % (i-1)) + 1;
			link.dest = getIp(192, 168, 0, to);
		}
	}
}

void calcRoutes() {
	
}

int main() {
	initRouter(5, 2);
	calcRoutes();
	return 0;
}
