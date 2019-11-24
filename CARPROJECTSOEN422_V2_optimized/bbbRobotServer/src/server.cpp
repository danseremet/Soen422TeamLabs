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

#include<fcntl.h>
#include<sys/ioctl.h>
#include<stdint.h>
#include<linux/spi/spidev.h>

#include <chrono>
#include <thread>

#define PORT 8080

#define SPI_PATH_LEFT "/dev/spidev1.0"
#define SPI_PATH_RIGHT "/dev/spidev2.0"

int server_fd;
int new_socket;
unsigned int fd_spidev1; // File handle on SPI 1 LEFT
unsigned int fd_spidev2; // File handle on SPI 2 RIGHT

void spi_setup() {    
    uint8_t bits = 8, mode = 0; // 8-bits per word, SPI mode 3 ??
    uint32_t speed = 10000; // Speed is 1 MHz
    
    fd_spidev1 = open(SPI_PATH_LEFT, O_RDWR);
    fd_spidev2 = open(SPI_PATH_RIGHT, O_RDWR);

    ioctl(fd_spidev1, SPI_IOC_WR_MODE, &mode);
    ioctl(fd_spidev1, SPI_IOC_RD_MODE, &mode);
    ioctl(fd_spidev1, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd_spidev1, SPI_IOC_RD_BITS_PER_WORD, &bits);
    ioctl(fd_spidev1, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    ioctl(fd_spidev1, SPI_IOC_RD_MAX_SPEED_HZ, &speed);

    ioctl(fd_spidev2, SPI_IOC_WR_MODE, &mode);
    ioctl(fd_spidev2, SPI_IOC_RD_MODE, &mode);
    ioctl(fd_spidev2, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd_spidev2, SPI_IOC_RD_BITS_PER_WORD, &bits);
    ioctl(fd_spidev2, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    ioctl(fd_spidev2, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
}

void server_setup() {	
	 
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
}

void car_action(char c) {
	// Turn left
	if (c == 'J' || c == 'K' || c == 'L') {
		char O = 'O';
		write(fd_spidev1, &O, 1);	// stop the left side
		write(fd_spidev2, &c, 1);	// move the right side forward
		printf("%c O spi1&spi2\n", c);		
	}
	// Turn right
	else if (c == 'U' || c == 'I' || c == 'O') {
		char L = 'L';
		write(fd_spidev2, &L, 1);	// stop the right side
		write(fd_spidev1, &c, 1);	// move left side forward
		printf("%c L spi1&spi2\n", c);
	}
	// All wheels forward/backward/stop
	else if (c == 'B' || c == 'N' || c == 'X') {
		write(fd_spidev1, &c, 1);
		write(fd_spidev2, &c, 1);
		printf("%c spi1&spi2\n", c);
	}
	fflush(stdout);
}

int main()  { 

	server_setup();
	spi_setup();
	int buflen = 1;
	char buffer[1] = {0};

	using namespace std::this_thread;
    using namespace std::chrono;
	
	// MAIN SERVER LOOP
	while(1) {
		bzero(buffer, buflen);
		read(new_socket, buffer, sizeof(buffer));
		char command = buffer[0];
		if (command == '\0') {
			close(server_fd);
			close(fd_spidev1);
			close(fd_spidev2);
			return 1;
		}

		car_action(command);
		
		
		sleep_for(0.1ms);
	}

	close(server_fd);
	close(fd_spidev1);
	close(fd_spidev2);
	return 0; 
} 
