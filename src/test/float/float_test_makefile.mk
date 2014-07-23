FLOAT_DEPENDENCIES=
FLOAT_SOURCE_LOCATIONS=

include ../src/generated/float/float_makefile.mk

float: $(FLOAT_DEPENDENCIES)
	$(CC) -o float_test $(FLOAT_DEPENDENCIES) ../src/test/float/float_test.o -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include $(FLOAT_SOURCE_LOCATIONS) -lglib-2.0 -lcheck -lm -lpthread -lrt