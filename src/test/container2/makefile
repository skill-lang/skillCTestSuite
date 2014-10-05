CC=gcc
CFLAGS= -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lglib-2.0 -lm -lpthread -lrt -fPIC -g -O0

container2: $(CONTAINER2_DEPENDENCIES) container2_so
	$(CC) -o container2_test $(CONTAINER2_DEPENDENCIES) ../src/test/container2/container2_test.c $(CONTAINER2_SOURCE_LOCATIONS) $(CFLAGS)

clean:
	rm -rf *.o

.PHONY: all clean