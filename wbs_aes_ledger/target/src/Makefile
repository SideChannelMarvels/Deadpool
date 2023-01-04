
all:
	g++ -Wall -O3 ctf.c -o ctf
	g++ -Wall -O3 patcher.c -o patcher
	./patcher ctf
	mv ctf-patched ctf2
	chmod +x ctf2

clean:
	rm -rf ctf patcher ctf2
