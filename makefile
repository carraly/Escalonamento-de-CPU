CFLAGS = -Wall -Wextra -g -fopenmp -pthread	

scheduler: main.o errors.o
	gcc $(CFLAGS) -o scheduler $^

%.o: %.c header.h
	gcc $(CFLAGS) -c $<

run: scheduler
	./scheduler

clean:
	rm -f *.o scheduler

.PHONY: run clean	