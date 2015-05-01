#include "commAsser.h"
#include "uart.h"

void sendConfig(const XBee & xbee,  int pid , int ad1 , int ad2, double val)
{

	TrameConfig tc;
	tc.conf.config = CODE_CONFIG;
	tc.conf.pid = pid;
	tc.conf.ad1 = ad1;
	tc.conf.ad2 = ad2;
	tc.conf.ent = (int)val;
	tc.conf.dec = (int)((val - (int)val)*100);


    printf("Envoi de : %X \n", tc);
	xbee.putChar(tc.l >> 24 );
    for(unsigned long i =0; i <100000000; i++);
	xbee.putChar(tc.l >> 16 );
	for(unsigned long i =0; i <100000000; i++);
	xbee.putChar(tc.l >> 8 );
	for(unsigned long i =0; i <10000000; i++);
	xbee.putChar(tc.l );

}
