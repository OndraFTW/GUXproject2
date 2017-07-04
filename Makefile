#
# Author: Ondřej Šlampa, xslamp01@stud.fit.vutbr.cz
# Projekt: GUX proj2 - 8puzzle
#

#překladač jazyka C
CC=gcc
#parametry překladače
CFLAGS= -std=c99 -Wall -pedantic -Wcast-qual -Wundef -O `pkg-config --cflags --libs gtk+-2.0`

#překlad hlavního programu
main:
	$(CC) puzzle.c $(CFLAGS) -o puzzle

