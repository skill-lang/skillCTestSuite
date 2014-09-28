BASICTYPES_DEPENDENCIES=
BASICTYPES_SOURCE_LOCATIONS=

include ../src/generated/basicTypes/basicTypes_makefile.mk

basicTypes: $(BASICTYPES_DEPENDENCIES) basicTypes_so
	$(CC) -o basicTypes_test $(BASICTYPES_DEPENDENCIES) ../src/test/basicTypes/basicTypes_test.c $(BASICTYPES_SOURCE_LOCATIONS) $(CFLAGS)