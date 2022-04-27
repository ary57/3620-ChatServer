FLAGS=-g

all: server client test_step1

clean:
	rm -f *.o server client test_step1

commands.o: commands.c
	gcc $(FLAGS) -c commands.c -o commands.o 

storage.o: storage.c
	gcc $(FLAGS) -c storage.c -o storage.o 

test_step1.c:
	gcc $(FLAGS) -c test_step1.c -o test_step1.o 

server: server.o commands.o storage.o
	gcc  -pthread server.o commands.o storage.o -o server

test_step1: test_step1.o storage.o
	gcc test_step1.o storage.o -o test_step1
	
client: client.o commands.o
	gcc  -pthread client.o commands.o -o client

server.o: server.c
	gcc $(FLAGS) -c server.c -o server.o 

client.o: client.c
	gcc $(FLAGS) -c client.c -o client.o
 