#include "headers.h"

int socket_create_and_connect(int port, char *ip_addr) {
	int fd = 0;
	int ret = 0;
	struct hostent *server_ptr;
	struct sockaddr_in server;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		socketperror("Error %d: at line %d: connect\n", fd, __LINE__);
		return fd;
	}

	if ((server_ptr = (struct hostent *) gethostbyname(ip_addr)) == NULL) {
		socketperror("Error %d: at line %d: gethostbyname\n", 0, __LINE__);
		return -1;
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = port;
	inet_pton(AF_INET, ip_addr, &(server.sin_addr));
	if ((ret = connect(fd, (struct sockaddr *)&server, sizeof(server))) < 0) {
		socketperror("Error %d: at line %d: gethostbyname\n", ret, __LINE__);
		return ret;
	}
	return fd;
}


	void
usage(char *prog)
{
	fprintf(stderr, "usage: %s <-s Server ip addr> <-p port number> \n", prog);
	exit(1);
}

int main(int argc, char *argv[]) {
	int fd = 0;
	int ret = 0, i = 0;
	uint packet_size = 0;
	uint port = 0;
	char *ip_addr = NULL;
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

	/* Fill the buffers with some data */
	//memset(_buffer, 'e', sizeof(uchar) * BUFF_SIZE);

	if ((fd = socket_create_and_connect(port, ip_addr)) < 0) {
		socketperror("Error %d: at line %d: socket_create_and_connect\n", fd,
				__LINE__);
		exit(1);
	}
	longtime min = INT_MAX, max = -1, sum = 0, start = 0, end = 0, diff = 0;

	printf("Downloading...\n");
	int n = TRANSFER_SIZE / BUFF_SIZE;
	start = get_current_time();
	for (i = 0; i < n; ++i) {
		if ((ret = read_full(fd, buffer, BUFF_SIZE))  != BUFF_SIZE) {
			printf("Error in reading = %d\n",ret);
		}
		// printf("Read %d th buffer\n", i);
	}
	end = get_current_time();

	long double download = (8 * (((long double)(TRANSFER_SIZE)) / (((long double)(end - start)) / BILLION
					)) / (1024*1024));

	memset(buffer, 'e', sizeof(uchar) * BUFF_SIZE);
	printf("Uploading...\n");
	start = get_current_time();
	for (i = 0; i < n; ++i) {
		if ((ret = write_full(fd, buffer, BUFF_SIZE))  != BUFF_SIZE) {
			printf("Error in writing = %d\n",ret);
		}
	}
	end = get_current_time();
	long double upload = (8 * (((long double)(TRANSFER_SIZE)) / (((long double)(end - start)) / BILLION
					)) / (1024*1024));

	printf("Download Speed: %Lf Mbps \nUpload Speed: %Lf Mbps\n", download, upload); 

	close(global_socketid);
	close(fd);
	return 0;
}
