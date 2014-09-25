CONTAINER1_DEPENDENCIES=
CONTAINER1_SOURCE_LOCATIONS=

include ../src/generated/container1/container1_makefile.mk

container1: $(CONTAINER1_DEPENDENCIES) container1_so
	$(CC) -o container1_test $(CONTAINER1_DEPENDENCIES) ../src/test/container1/container1_test.c $(CONTAINER1_SOURCE_LOCATIONS) $(CFLAGS)