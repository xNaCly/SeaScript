sealib = $(shell find  ./sealib -maxdepth 1 -name "*.c")
test = $(shell find ./sealib/test -name "*.c")

flags = -Wall \
		-std=c11 \
		-g \
		-O2
make:
	go run . -i main.sea

test:
	$(CC) $(sealib) $(test) $(flags) -o test.out
	./test.out
