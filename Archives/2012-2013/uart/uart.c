/************************************************
 * UN CODE CA SE COMMENTE !!!!!!!!!!!!!!!!!!
 * **********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
    int fd = open("/dev/ttyAMA0", O_WRONLY | O_NOCTTY | O_NDELAY);
    char send[2];
    send[0] = 0xAA;
    send[1] = 0xAA;
    if(fd == -1)
    {
        printf("Impossible d'ouvri AMA0");
        exit(0);
    }
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR | ICRNL;
    options.c_oflag = 0;
    tcflush (fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    while (1)
    {
        int z = write(fd, send,2);
        if(z <0)
        {
            printf("Impossible d'envoyer le message");
            exit(0);
        }
        //  printf("Send");
        for(int i =0; i <1000000 ;i++);
    }

    return 0;
}
