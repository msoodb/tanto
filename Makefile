CC = cc
CFLAGS = -Wall -Wextra -pedantic

example:
	mkdir -p examples/bin
	$(CC) $(CFLAGS) -o examples/bin/example1 examples/example1.c
	$(CC) $(CFLAGS) -o examples/bin/example2 examples/example2.c

clean:
	rm -rf examples/bin
	rm -rf bin
	rm -rf tests/result

test:
	mkdir -p tests/result
	$(CC) $(CFLAGS) -o tests/result/tanto_unittest tests/tanto_unittest.c
	./tests/result/tanto_unittest -v
