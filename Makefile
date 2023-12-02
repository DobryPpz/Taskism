all:
	gcc `find . -type f -iname "*.c"` -Wall -o taskism