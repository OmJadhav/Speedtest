#include "headers.h"

int socket_create_and_accept(int port, char *ip_addr) {
	/* Create a socket for local communication (within the same device) */
	int option = 1;
	int err = 0;
	struct sockaddr_in server;
	int sfd = global_socketid = socket(AF_INET, SOCK_STREAM, 0);

	struct hostent *server_ptr;

	if (sfd < 0) {
		socketperror("Error %d: at line %d: socket creation\n", sfd, __LINE__);
		exit(1);
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = port;
	inet_pton(AF_INET, ip_addr, &(server.sin_addr));
	//	server.sin_addr.s_addr = htonl(INADDR_ANY);
	/* Bind */
	if ((err = bind(sfd, (struct sockaddr *) &server, sizeof(server))) < 0) {
		socketperror("Error %d: at line %d: bind\n", err, __LINE__);
		exit(1);
	}

	/* Listen */
	if ((err = listen(sfd, 1)) < 0) {
		socketperror("Error %d: at line %d: listen\n", err, __LINE__);
		exit(1);
	}
	/* accpet */
	if ((err = accept(sfd, NULL, NULL)) < 0) {
		socketperror("Error %d: at line %d: accept\n", err, __LINE__);
		exit(1);
	}

	/* return the client fd */
	return err;
}


	void
usage(char *prog)
{
	fprintf(stderr, "usage: %s <-s Server ip addr> <-p port number> \n", prog);
	exit(1);
}

int main(int argc, char *argv[]) {
	int fd = -1;
	int i = 0, j = 0;
	int ret = 0;
	uint port = -1;
	char *ip_addr  = NULL;
	uchar buffer[BUFF_SIZE];

	int c;
	opterr = 0;
	while ((c = getopt(argc, argv, "s:p:")) != -1) {
		switch (c) {
			case 's':
				ip_addr = strdup(optarg);
				break;
			case 'p':
				port = atoi(optarg);
				break;
			default:
				usage(argv[0]);
				exit(0);
		}
	}
	if (port == -1 || ip_addr == NULL) {
		usage(argv[0]);
		exit(0);
	}
	fd = socket_create_and_accept(port, ip_addr);

	int n = TRANSFER_SIZE / BUFF_SIZE;
	memset(buffer, 'e', sizeof(uchar) * BUFF_SIZE);
	for (i = 0; i < n; ++i) {
		if((ret = write_full(fd, buffer, BUFF_SIZE)) != BUFF_SIZE) {
			printf("error while writing \n");
			fflush(stdout);
		}
	}

	for (i = 0; i < n; ++i) {
		if((ret = read_full(fd, buffer, BUFF_SIZE)) != BUFF_SIZE) {
			printf("error in reading \n");
		}
	}
	close(global_socketid);
	close(fd);
	return 0;
}
