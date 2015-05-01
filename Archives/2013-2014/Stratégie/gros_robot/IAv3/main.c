/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <device.h>
#include "init.h"
#include "machine.h"
#include "comAsser.h"
#include "comXBee.h"

extern volatile int finMatch;

//TODO : ISR FIN

void main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    initPhase1();
    finMatch = 0;
    CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
    
    initPhase2();
    CyDelay(2000);
    //printXBee("INIT_DONE", 9);
    while(Depart_Read() == 1);
    
    
    /*int i;
    
    for(i = 0;i<5;i++){
        sendDelta(1000);
        waitDoneAsser();
        sendAlphaAbs(90);
        waitDoneAsser();
        sendDelta(1000);
        waitDoneAsser();
        sendAlphaAbs(180);
        waitDoneAsser();
        sendDelta(1000);
        waitDoneAsser();
        sendAlphaAbs(270);
        waitDoneAsser();
        sendDelta(1000);
        waitDoneAsser();
        sendAlphaAbs(0);
        waitDoneAsser();
        }*/
        
    
    for(;;)
    {
        /* Place your application code here. */
        
        machine();
        
        //sendXBee(0x0);
        //sendTiretteRobot2();
        //CyDelay(500);
    }
}

/* [] END OF FILE */
