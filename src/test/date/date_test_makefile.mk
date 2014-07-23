DATE_DEPENDENCIES=
DATE_SOURCE_LOCATIONS=

include ../src/generated/date/date_makefile.mk

date: $(DATE_DEPENDENCIES)
	$(CC) -o date_test $(DATE_DEPENDENCIES) ../src/test/date/date_test.o -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include $(DATE_SOURCE_LOCATIONS) -lglib-2.0 -lcheck -lm -lpthread -lrt