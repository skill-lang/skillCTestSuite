FLOAT_DEPENDENCIES=
FLOAT_SOURCE_LOCATIONS=

include ../src/generated/float/float_makefile.mk

float: $(FLOAT_DEPENDENCIES)
	$(CC) -o float_lib.so $(FLOAT_DEPENDENCIES) $(FLOAT_SOURCE_LOCATIONS) $(CFLAGS) -shared
	$(CC) -o float_test  ../src/test/float/float_test.c float_lib.so $(CFLAGS)