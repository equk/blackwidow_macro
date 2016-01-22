bwidow: src/bwidow.c
	gcc -std=gnu99 src/bwidow.c -lusb-1.0 -o bin/bwidow

install: bwidow
	cp bin/bwidow /usr/bin/bwidow
	chmod +x /usr/bin/bwidow

uninstall:
	rm /usr/bin/bwidow

clean:
	rm bin/bwidow
