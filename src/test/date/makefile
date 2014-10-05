CC=gcc
CFLAGS= -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lglib-2.0 -lm -lpthread -lrt -fPIC -g -O0

date:
	$(CC) -o date_test date_test.c $(CFLAGS)

clean:
	rm -rf *.o

.PHONY: all clean