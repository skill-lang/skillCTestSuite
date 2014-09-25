CONTAINER2_DEPENDENCIES=
CONTAINER2_SOURCE_LOCATIONS=

include ../src/generated/container2/container2_makefile.mk

container2: $(CONTAINER2_DEPENDENCIES) container2_so
	$(CC) -o container2_test $(CONTAINER2_DEPENDENCIES) ../src/test/container2/container2_test.c $(CONTAINER2_SOURCE_LOCATIONS) $(CFLAGS)