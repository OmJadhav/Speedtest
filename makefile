all:
	gcc -O server.c -o server
	gcc -O client.c -o client

clean:
	rm server client
