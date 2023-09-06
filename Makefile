sealib = $(shell find ./sealib -name "*.c")

test:
	$(CC) $(sealib) -o test.out
	./test.out
