sealib = $(shell find ./sealib -name "*.c")

test:
	$(CC) $(sealib) -g -std=c11 -O2 -lm -o test.out
	./test.out
