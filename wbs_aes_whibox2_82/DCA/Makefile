printrounds.c: ../target/82.c printrounds.patch
	patch ../target/82.c -i printrounds.patch -o printrounds.c

printrounds: printrounds.c ../target/main.c
	gcc -g -O3 printrounds.c ../target/main.c -o printrounds

rounds.trace: printrounds.py printrounds
	python2 printrounds.py

ifs.c: analyse.py rounds.trace
	python2 analyse.py > ifs.c

tracetransitions.c: ../target/82.c tracetransitions.patch ifs.c
	patch ../target/82.c -i tracetransitions.patch -o tracetransitions.c
	sed -i -e '/INSERTHERE/r ifs.c' tracetransitions.c

tracetransitions: tracetransitions.c ../target/main.c
	gcc -g -O3 tracetransitions.c ../target/main.c -o tracetransitions

attack.config: trace_it.py tracetransitions
	python2 trace_it.py

.PHONY: attack
attack: attack.config
	daredevil -c attack.config
