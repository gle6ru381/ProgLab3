coder: $(patsubst src/%.c, build/%.o,$(wildcard src/*.c))
	gcc $^ -o bin/$@

build/%.o: src/%.c
	gcc -o $@ -c $<

clean:
	rm src/*.o
