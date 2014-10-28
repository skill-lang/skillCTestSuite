SUBDIRS = date basicTypes float node container1 container2 subtypes annotation
PREFIX_SUBDIRS = date_prefix subtypes_prefix

CC=gcc
CFLAGS= -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lglib-2.0 -lm -lpthread -lrt -fPIC

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C ../src/generated/$@
	$(CC) -static ../src/test/$@_test.c -L../src/generated/$@ -I../src/generated/$@ -lapi $(CFLAGS) -o $@_test

prefix_subdirs: $(PREFIX_SUBDIRS)
	$(CC) -static ../src/test/prefix_test.c -L../src/generated/date_prefix -L../src/generated/subtypes_prefix -I../src/generated/date_prefix -I../src/generated/subtypes_prefix -lsub_api -ldate_api $(CFLAGS) -o prefix_test

$(PREFIX_SUBDIRS):
	$(MAKE) -C ../src/generated/$@

run:
	./date_test
	./basicTypes_test
	./float_test
	./node_test
	./container1_test
	./container2_test
	./subtypes_test
	./annotation_test
	./prefix_test

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C../src/generated/$$dir clean; \
		$(MAKE) -C../src/test/$$dir clean; \
		rm -rf $${dir}_test; \
	done
	for dir in $(PREFIX_SUBDIRS); do \
		$(MAKE) -C../src/generated/$$dir clean; \
		$(MAKE) -C../src/test/$$dir clean; \
	done

all: subdirs prefix_subdirs run

.PHONY: all run clean subdirs $(SUBDIRS) prefix_subdirs $(PREFIX_SUBDIRS)