CC=gcc
CFLAGS= -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lglib-2.0 -lm -lpthread -lrt -fPIC -g -O0

appending: $(APPENDING_DEPENDENCIES) appending_so
	$(CC) -o appending_test $(APPENDING_DEPENDENCIES) ../src/test/appending/appending_test.c $(APPENDING_SOURCE_LOCATIONS) $(CFLAGS)

clean:
	rm -rf *.o

.PHONY: all clean