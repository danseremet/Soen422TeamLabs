#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<getopt.h>
#include<sys/ioctl.h>
#include<stdint.h>
#include<stdlib.h>
#include<linux/spi/spidev.h>

#define SPI_PATH_LEFT "/dev/spidev1.0"
#define SPI_PATH_RIGHT "/dev/spidev2.0"

char leftRight;
char frontBack;
char forwardBackward;
int speed;


void send_letters(char startLetter, char endLetter, int fd) {
    char letter = startLetter;
    while (letter <= endLetter) {
        write(fd, &letter, 1);  // write a single a character
        fflush(stdout);         // need to flush the output
        letter++;
    }
}

void send_motor_control(char frontBack, char forwardBackward, int speed, int fd) {
    // Send f/b for front or back motor 
    write(fd, &frontBack, 1);
    fflush(stdout);

    // Send F/B for forward or backwards direction
    write(fd, &forwardBackward, 1);
    fflush(stdout);
    
    char buf[3];
    sprintf(buf, "%03d", speed);
    for (int i = 0; i < 3; i++) {
        write(fd, &buf[i], 1);  // write a single a character
        fflush(stdout);         // need to flush the output
    }

    // Send X to stop the message and execute the command
    char stopChar = 'X';
    write(fd, &stopChar, 1);
    fflush(stdout);
}

int spi_setup() {
    unsigned int fd; // file handle
    
    uint8_t bits = 8, mode = 0; // 8-bits per word, SPI mode 3 ??
    uint32_t speed = 10000; // Speed is 1 MHz
    // The following calls set up the SPI bus properties
    if (leftRight == 'L') {
        fd = open(SPI_PATH_LEFT, O_RDWR);
    } else {
        fd = open(SPI_PATH_RIGHT, O_RDWR);
    }
    
    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_RD_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    return fd;
}


static void parse_command(int argc, char *argv[]) {
    if(argc == 5) {

	    leftRight = argv[1][0];                 // L/R      Left or Right motors
        frontBack = argv[2][0];                 // f/b      front or back motor
        forwardBackward = argv[3][0];           // F/B      Forward or Backwards
        speed = atoi(argv[4]);                  // 0-255    Speed


    } else {
        perror("Must supply valid args.");
	    abort();
    }
}


int main(int argc, char *argv[]) {
    /* L f F 0      Left front motor forward speed = 0
     * L b B 240    Left bottom motor backwards speed = 240
     * R f B 17     Right front motor backwards speed = 17
     * R b F 123    Right bottom motor forward speed = 123
     */
    parse_command(argc, argv);
    printf("%c %c %c %03d\n", leftRight, frontBack, forwardBackward, speed);

    int fd = spi_setup();   // config & open spi (left or right motors)
    send_motor_control(frontBack, forwardBackward, speed, fd);
    
    close(fd); // close the file
    return 0;   
}