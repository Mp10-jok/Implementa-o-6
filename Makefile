all: banker

banker: mrpf/banker.c
	gcc -Wall -Wextra -o banker mrpf/banker.c

clean:
	rm -f banker result.txt mrpf/banker mrpf/result.txt
