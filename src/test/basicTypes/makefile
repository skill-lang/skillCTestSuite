CC=gcc
CFLAGS= -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lglib-2.0 -lm -lpthread -lrt -fPIC -g -O0

basicTypes: $(BASICTYPES_DEPENDENCIES) basicTypes_so
	$(CC) -o basicTypes_test $(BASICTYPES_DEPENDENCIES) ../src/test/basicTypes/basicTypes_test.c $(BASICTYPES_SOURCE_LOCATIONS) $(CFLAGS)

clean:
	rm -rf *.o

.PHONY: all clean