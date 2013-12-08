all: routing

routing: routing.o router.o
	g++ routing.o router.o -o routing

clean:
	rm -rf routing routing.o router.o
