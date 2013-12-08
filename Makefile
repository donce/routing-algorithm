all: routing

routing.o: routing.cpp routing.h router.h packet.h

router.o: router.cpp routing.h router.h packet.h

routing: routing.o router.o
	g++ routing.o router.o -o routing

clean:
	rm -rf routing routing.o router.o
