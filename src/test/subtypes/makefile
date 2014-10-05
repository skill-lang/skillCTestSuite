all: $(SUBTYPES_DEPENDENCIES) subtypes_so
	$(CC) -o subtypes_test $(SUBTYPES_DEPENDENCIES) ../src/test/subtypes/subtypes_test.c $(SUBTYPES_SOURCE_LOCATIONS) $(CFLAGS)

clean:
	rm -rf *.o

.PHONY: all clean