

#include <stdlib.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "lB20380.h"



int B20380_Open(char* devname){
	speed_t speed = 9600;
	struct termios termios_p;
	int fd = open( devname, O_RDWR | O_NOCTTY );
    if(-1 == fd ){
    	//printf("Error open [%s] %s\n", devname, strerror (errno));
    	return( -1 );
    }	
	if(-1 == tcflush( fd,TCIOFLUSH ))
		return(-1);
    if( -1 == tcgetattr( fd, &termios_p ) )
      return( -1 );
	// control mode flags
  /*  termios_p.c_cflag |= (CLOCAL | CREAD); // MustBe
    // 8N0    
	termios_p.c_cflag &= ~PARENB;
	//termios_p.c_cflag |= PARODD;
	termios_p.c_cflag &= ~CSTOPB;
	termios_p.c_cflag &= ~CSIZE;
	termios_p.c_cflag |= CS8;

	termios_p.c_cflag |= (IHFLOW | OHFLOW) ;

	//local mode flags
   	termios_p.c_lflag &= ~(ECHO | ICANON | ISIG | ECHOE | ECHOK | ECHONL);

	// set raw 
	termios_p.c_oflag &= ~OPOST; 

    // input mode flags
	termios_p.c_iflag &= ~INPCK;//Enabled Parity
	//termios_p.c_iflag |= INPCK;//Enabled Parity
	
	// control characters
    termios_p.c_cc[VMIN] = 1;
    termios_p.c_cc[VTIME] = 5;

    cfsetispeed(&termios_p, speed);
    cfsetospeed(&termios_p, speed);
    
    */
    
    termios_p.c_cflag |= (CLOCAL | CREAD); // MustBe
    // 8N0    
  	termios_p.c_cflag &= ~PARENB;
//  	termios_p.c_cflag |= PARODD;
  	termios_p.c_cflag &= ~CSTOPB;
  	termios_p.c_cflag &= ~CSIZE;
  	termios_p.c_cflag |= CS8;

//  	termios_p.c_cflag |= (IHFLOW | OHFLOW) ;
	termios_p.c_cflag &= ~(IHFLOW | OHFLOW) ;

  	//local mode flags
   	termios_p.c_lflag &= ~(ECHO | ICANON | ISIG | ECHOE | ECHOK | ECHONL);

  	// set raw 
  	termios_p.c_oflag &= ~OPOST; 

    // input mode flags
  	termios_p.c_iflag &= ~INPCK;//Enabled Parity
  	//termios_p.c_iflag |= INPCK;//Enabled Parity
  	
  	// control characters
    termios_p.c_cc[VMIN] = 4;
    termios_p.c_cc[VTIME] = 20;

    
    
    cfsetispeed(&termios_p, speed);
    cfsetospeed(&termios_p, speed);
    if(-1 == tcsetattr( fd, TCSANOW, &termios_p ))
    	return(-1);
    return( fd );
}


