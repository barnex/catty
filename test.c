#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

void fatal(const char* msg){
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

int main(int argc, char** argv){
	if(argc != 3){
		fatal("expecting 2 arguments: ttydevice baudrate");
	}

	char* file = argv[1];
	int fd = open(file, O_RDWR | O_NOCTTY | O_NDELAY);
	if(fd == -1){
		fprintf(stderr, "cannot open: %s: %s\n", file, strerror(errno));
		exit(1);
	}

	int baud = atoi(argv[2]);
	if(baud == 0){
		fprintf(stderr, "invalid baud rate: %s\n", argv[2]);
		exit(1);
	}

	

//	struct termios  config;
//	
//	if(!isatty(fd)) { ... error handling ... }
//	if(tcgetattr(fd, &config) < 0) { ... error handling ... }
//	//
//	// Input flags - Turn off input processing
//	// convert break to null byte, no CR to NL translation,
//	// no NL to CR translation, don't mark parity errors or breaks
//	// no input parity check, don't strip high bit off,
//	// no XON/XOFF software flow control
//	//
//	config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
//	                    INLCR | PARMRK | INPCK | ISTRIP | IXON);
//	//
//	// Output flags - Turn off output processing
//	// no CR to NL translation, no NL to CR-NL translation,
//	// no NL to CR translation, no column 0 CR suppression,
//	// no Ctrl-D suppression, no fill characters, no case mapping,
//	// no local output processing
//	//
//	// config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
//	//                     ONOCR | ONOEOT| OFILL | OLCUC | OPOST);
//	config.c_oflag = 0;
//	//
//	// No line processing:
//	// echo off, echo newline off, canonical mode off, 
//	// extended input processing off, signal chars off
//	//
//	config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
//	//
//	// Turn off character processing
//	// clear current char size mask, no parity checking,
//	// no output processing, force 8 bit input
//	//
//	config.c_cflag &= ~(CSIZE | PARENB);
//	config.c_cflag |= CS8;
//	//
//	// One input byte is enough to return from read()
//	// Inter-character timer off
//	//
//	config.c_cc[VMIN]  = 1;
//	config.c_cc[VTIME] = 0;
//	//
//	// Communication speed (simple version, using the predefined
//	// constants)
//	//
//	if(cfsetispeed(&config, B9600) < 0 || cfsetospeed(&config, B9600) < 0) {
//	    ... error handling ...
//	}
//	//
//	// Finally, apply the configuration
//	//
//	if(tcsetattr(fd, TCSAFLUSH, &config) < 0) { ... error handling ... }
	return 0;	
}
