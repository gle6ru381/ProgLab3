coder: $(patsubst src/%.c, build/%.o,$(wildcard src/*.c))
	gcc $^ -g3 -o bin/$@

build/%.o: src/%.c
	gcc -o $@ -g3 -c $<

clean:
	rm build/*.o
