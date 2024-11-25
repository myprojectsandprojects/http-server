#include <stdio.h>
#include <sys/socket.h>
#include <assert.h>

#include <netinet/in.h>

#include <unistd.h>

#include <string.h>

int main()
{
	// int Char = getchar();
	// printf("%c\n", Char);

	int Socket = socket(AF_INET, SOCK_STREAM, 0);
	if(Socket == -1) {
		assert(0);
	}
	printf("Socket = %d\n", Socket);

	struct sockaddr_in Address;
	Address.sin_family = AF_INET;
	Address.sin_port = htons(8080);
	Address.sin_addr.s_addr = htonl(INADDR_ANY);
	// Address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	int Attempts = 64;
	while(Attempts > 0) {
		if(bind(Socket, (struct sockaddr *)&Address, sizeof(Address)) == 0) {
			break;
		}
		Attempts -= 1;
		printf("bind() failed. %d attempts remaining...\n", Attempts);
		sleep(1);
	}
	if(Attempts == 0) {
		assert(0);
	} else {
		printf("bind() ok\n");
	}

	if(listen(Socket, 0) == -1) {
		assert(0);
	}
	printf("listen() ok\n");

	struct sockaddr ClientAddress; //@ zero?
	socklen_t AddressLength = sizeof(ClientAddress);
	int ConnectedSocket = accept(Socket, &ClientAddress, &AddressLength);
	if(ConnectedSocket == -1) {
		assert(0);
	}
	printf("accept() = %d\n", ConnectedSocket);

	char Message[1024];
	ssize_t BytesRead = read(ConnectedSocket, Message, 1024);
	printf("received (%ld bytes):\n%s\n", BytesRead, Message);

	const char *Response = "HTTP/1.1 200 OK\n\n<h1>Welcome!</h1>";
	write(ConnectedSocket, Response, strlen(Response));

	return 0;
}