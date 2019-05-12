all: src/*.c
	$(CC) -shared -o libg13.so -fPIC -g $^ -lusb-1.0 -lm -Wall

install:
	cp libg13.so /usr/lib/
	cp src/libg13.h /usr/include/
