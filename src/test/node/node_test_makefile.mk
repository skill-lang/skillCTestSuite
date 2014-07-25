NODE_DEPENDENCIES=
NODE_SOURCE_LOCATIONS=

include ../src/generated/node/node_makefile.mk

node: $(NODE_DEPENDENCIES)
	$(CC) -o node_test $(NODE_DEPENDENCIES) ../src/test/node/node_test.c $(NODE_SOURCE_LOCATIONS) $(CFLAGS)