CC=gcc
CFLAGS= -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lglib-2.0 -lm -lpthread -lrt -fPIC -g -O0

include ../src/test/date/date_test_makefile.mk
include ../src/test/basicTypes/basicTypes_test_makefile.mk
include ../src/test/float/float_test_makefile.mk
include ../src/test/node/node_test_makefile.mk
include ../src/test/subtypes/subtypes_test_makefile.mk
include ../src/test/container1/container1_test_makefile.mk
include ../src/test/container2/container2_test_makefile.mk
include ../src/test/appending/appending_test_makefile.mk

all: date basicTypes float node container1 container2 subtypes appending run

run:
	./date_test
	./basicTypes_test
	./float_test
	./node_test
	./container1_test
	./container2_test
	./subtypes_test
	./appending_test

.PHONY: all run