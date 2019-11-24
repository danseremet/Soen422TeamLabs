// Server
#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <string>
#include <cstring>
#include <chrono>
#include <thread>

#define PORT 8080 

void motor_action(std::string args) {
    std::string commandStr = args.insert(0, "./SpiWrite ");
    const char *command = commandStr.c_str();
	std::cout << "Running: " << command << std::endl;
    system(command);
}

int main(int argc, char const *argv[])  { 
	int server_fd, new_socket, clientInput; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 

	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 

	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 

	if (listen(server_fd, 3) < 0) { 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) { 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	
	int buflen = 40;
	char buffer[40] = {0};
	long i;
	using namespace std::this_thread;
    using namespace std::chrono;
	
	// MAIN SERVER LOOP
	while(1) {
		bzero(buffer, buflen);
		clientInput = read(new_socket, buffer, sizeof(buffer));
		// printf("i: %d,  m: %s\n", i++, buffer);	// debug

		if (buffer[0] == '\0') {
			
			return 1;
		}
		motor_action(buffer);
		bzero(buffer, buflen);
		sleep_for(0.1ms);

	}

	close(server_fd);
	return 0; 
} 
