SUBTYPES_DEPENDENCIES=
SUBTYPES_SOURCE_LOCATIONS=

include ../src/generated/subtypes/subtypes_makefile.mk

subtypes: $(SUBTYPES_DEPENDENCIES) subtypes_so
	$(CC) -o subtypes_test $(SUBTYPES_DEPENDENCIES) ../src/test/subtypes/subtypes_test.c $(SUBTYPES_SOURCE_LOCATIONS) $(CFLAGS)