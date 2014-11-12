SUBDIRS = date basic_types empty const auto float node container1 container_user_type container_annotation container_string subtypes annotation
PREFIX_SUBDIRS = date_prefix subtypes_prefix

CC=gcc
CFLAGS= -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lglib-2.0 -lm -lpthread -lrt -fPIC -std=c99 -pedantic-errors -finput-charset=utf-8

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
	./basic_types_test
	./empty_test
	./const_test
	./auto_test
	./float_test
	./node_test
	./container1_test
	./container_user_type_test
	./container_annotation_test
	./container_string_test
	./subtypes_test
	./annotation_test
	./prefix_test

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C../src/generated/$$dir clean; \
		rm -rf $${dir}_test; \
	done
	for dir in $(PREFIX_SUBDIRS); do \
		$(MAKE) -C../src/generated/$$dir clean; \
	done
	rm -rf prefix_test;

all: subdirs prefix_subdirs run

.PHONY: all run clean subdirs $(SUBDIRS) prefix_subdirs $(PREFIX_SUBDIRS)