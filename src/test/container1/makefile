CC=gcc
CFLAGS= -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lglib-2.0 -lm -lpthread -lrt -fPIC -g -O0

container1: $(CONTAINER1_DEPENDENCIES) container1_so
	$(CC) -o container1_test $(CONTAINER1_DEPENDENCIES) ../src/test/container1/container1_test.c $(CONTAINER1_SOURCE_LOCATIONS) $(CFLAGS)

clean:
	rm -rf *.o

.PHONY: all clean