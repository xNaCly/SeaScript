sealib = $(shell find  ./sealib -maxdepth 1 -name "*.c")
test = $(shell find ./sealib/test -name "*.c")

flags = -Wall \
		-std=c11 \
		-g \
		-O2
make:
	mkdir -p gen
	go run . -i examples/main.sea -o gen/main.c -force -debug
	$(CC) gen/main.c $(flags) -o gen/a.out
	./gen/a.out

test:
	$(CC) $(sealib) $(test) $(flags) -o test.out
	./test.out
