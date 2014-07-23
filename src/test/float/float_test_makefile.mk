FLOAT_DEPENDENCIES=
FLOAT_SOURCE_LOCATIONS=

include ../src/generated/float/float_makefile.mk

float: $(FLOAT_DEPENDENCIES)
	$(CC) -o float_test $(FLOAT_DEPENDENCIES) ../src/test/float/float_test.c $(FLOAT_SOURCE_LOCATIONS) $(CFLAGS)