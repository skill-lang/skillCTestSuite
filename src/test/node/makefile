node: $(NODE_DEPENDENCIES) node_so
	$(CC) -o node_test $(NODE_DEPENDENCIES) ../src/test/node/node_test.c $(NODE_SOURCE_LOCATIONS) $(CFLAGS)

clean:
	rm -rf *.o

.PHONY: all clean