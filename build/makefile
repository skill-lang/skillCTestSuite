SUBDIRS = date basicTypes float node container1 container2 subtypes appending

CC=gcc
CFLAGS= -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lglib-2.0 -lm -lpthread -lrt -fPIC -g -O0

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C ../src/generated/$@
	$(CC) -static ../src/test/$@_test.c -L../src/generated/$@ -I../src/generated/$@ -lapi $(CFLAGS) -o $@_test 

run:
	./date_test
	./basicTypes_test
	./float_test
	./node_test
	./container1_test
	./container2_test
	./subtypes_test
	./appending_test

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C../src/generated/$$dir clean; \
		$(MAKE) -C../src/test/$$dir clean; \
		rm -rf $${dir}_test; \
	done

all: subdirs run

.PHONY: all run clean subdirs $(SUBDIRS)