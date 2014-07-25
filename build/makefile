CC=gcc
CFLAGS= -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lglib-2.0 -lcheck -lm -lpthread -lrt

include ../src/test/date/date_test_makefile.mk
include ../src/test/float/float_test_makefile.mk
include ../src/test/node/node_test_makefile.mk

all: date float node run

run:
	./date_test
	./float_test
	./node_test

.PHONY: all run