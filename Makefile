build:
	mkdir build
	gcc -c src/list.c -Wall -o build/list.o
	gcc -c src/main.c -Wall -o build/main.o
	gcc -c src/stringutils.c -Wall -o build/stringutils.o
	gcc -c src/task.c -Wall -o build/task.o
	gcc build/list.o \
	build/main.o \
	build/stringutils.o \
	build/task.o \
	-o taskism
clean:
	rm -rf build
	rm taskism