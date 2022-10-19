all : scheduler simulator

scheduler : scheduler.c
	gcc -o scheduler scheduler.c



simulator : simulator.c
	gcc -pthread -o simulator simulator.c