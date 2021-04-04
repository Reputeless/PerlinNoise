CC=clang

.PHONY: clean

example: example.cpp
	$(CC) -std=c++17 $^ -o $@ -lm -lstdc++

clean:
	rm -rf example
