APPENDING_DEPENDENCIES=
APPENDING_SOURCE_LOCATIONS=

include ../src/generated/appending/appending_makefile.mk

appending: $(APPENDING_DEPENDENCIES) appending_so
	$(CC) -o appending_test $(APPENDING_DEPENDENCIES) ../src/test/appending/appending_test.c $(APPENDING_SOURCE_LOCATIONS) $(CFLAGS)