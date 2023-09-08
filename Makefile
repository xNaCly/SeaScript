sealib = $(shell find  ./sealib -maxdepth 1 -name "*.c")
test = $(shell find ./sealib/test -name "*.c")

flags = -Wall \
		-std=c11 \
		-g \
		-O2
make:
	mkdir -p gen
	go run . -i examples/main.sea -o gen/main.c -force -debug
	cat gen/main.c

test:
	$(CC) $(sealib) $(test) $(flags) -o test.out
	./test.out
