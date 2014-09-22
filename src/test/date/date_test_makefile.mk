include ../src/generated/date/date_makefile.mk

date: $(DATE_DEPENDENCIES) date_so
	$(CC) -o date_test $(DATE_DEPENDENCIES) ../src/test/date/date_test.c $(DATE_SOURCE_LOCATIONS) $(CFLAGS)