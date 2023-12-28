build:
	gcc -c src/list.c -Wall -o list.o
	gcc -c src/main.c -Wall -o main.o
	gcc -c src/stringutils.c -Wall -o stringutils.o
	gcc -c src/task.c -Wall -o task.o
	gcc list.o \
	main.o \
	stringutils.o \
	task.o \
	-o taskism
clean:
	rm `find . -type f -iname "*.o"`
	rm taskism