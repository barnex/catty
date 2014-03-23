/*
 * This program pipes a tty to stdout.
 * arguments: ttydevice, baud rate
 */
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

// pair of baudrate + speed code
typedef struct {
	int x, y;
} int2;

// maps human-readable baud rate on speed code
int2 baudTable[18] = { {50     ,B50},
	{75     ,B75},
	{110    ,B110},
	{134    ,B134},
	{150    ,B150},
	{200    ,B200},
	{300    ,B300},
	{600    ,B600},
	{1200   ,B1200},
	{1800   ,B1800},
	{2400   ,B2400},
	{4800   ,B4800},
	{9600   ,B9600},
	{19200  ,B19200},
	{38400  ,B38400},
	{57600  ,B57600},
	{115200 ,B115200},
	{230400 ,B230400}
};

// return speed code for baud rate, or -1 if baud rate not in baudTable
int decodeBaud(int rate){
	int NBAUD = sizeof(baudTable)/sizeof(baudTable[0]);
	int i = 0;

	for(i=0; i<NBAUD; i++){
		if (baudTable[i].x == rate){
			return baudTable[i].y;
		}
	}
	return -1; // not found
}

int main(int argc, char** argv) {

	char* prog = argv[0];

	if(argc != 3) {
		fprintf(stderr, "%s: expecting 2 arguments: ttydevice, baudrate\n", prog);
		exit(1);
	}

	// parse baud rate
	int baudCode = atoi(argv[2]);
	int baudRate = decodeBaud(baudCode);
	if(baudRate <= 0) {
		fprintf(stderr, "%s: invalid baud rate: %s\n", prog, argv[2]);
		exit(1);
	}
	decodeBaud(baudCode);

	// parse file argument
	char* file = argv[1];
	int fd = open(file, O_RDWR | O_NOCTTY | O_SYNC);
	if(fd == -1) {
		fprintf(stderr, "%s: cannot open: %s: %s\n", prog, file, strerror(errno));
		exit(1);
	}

	if(!isatty(fd)) {
		fprintf(stderr, "%s: not a tty: %s\n", prog, file);
		exit(1);
	}

	// setup tty
	struct termios config;
	if(tcgetattr(fd, &config) < 0) {
		fprintf(stderr, "%s: tcgetattr %s: %s\n", prog, file, strerror(errno));
		exit(1);
	}
	cfmakeraw(&config);
	config.c_cflag &= ~(CSIZE | PARENB);
	config.c_cflag |= CS8;

	// buffer as little as possible
	config.c_cc[VMIN]  = 1;
	config.c_cc[VTIME] = 1;

	// Communication speed
	if(cfsetispeed(&config, baudRate) < 0 || cfsetospeed(&config, baudRate) < 0) {
		fprintf(stderr, "%s: setspeed %d: %s\n", prog, baudRate, strerror(errno));
		exit(1);
	}

	if(tcsetattr(fd, TCSANOW, &config) != 0) {
		fprintf(stderr, "%s: tcsetattr: %s\n", prog, strerror(errno));
		exit(1);
	}

#define BUFSIZE 2048

	char buf[BUFSIZE+1];
	for (;;) {
		int n = read(fd, &buf[0], BUFSIZE);
		buf[n] = 0; // terminate string
		if (n > 0) {
			printf("%s", &buf[0]);
			fflush(stdout);
		} else {
			usleep(1000);
		}
	}
	return 0;
}
