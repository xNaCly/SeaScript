sealib = $(shell find ./sealib -name "*.c")

test:
	$(CC) $(sealib) -O2 -lm -o test.out
	./test.out
