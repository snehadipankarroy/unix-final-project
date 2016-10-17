# Creates executable
queue : queue.o
	gcc -o queue queue.o -pthread

# Compiles .c file into object code
queue.o : queue.c
	gcc -c queue.c -pthread

# Removes all object files
clean :
	rm *.o
