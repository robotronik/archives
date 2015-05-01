

static int initXBeeAndCom(void);
static void closeXBeeAndCom(void);
int readXBee(HANDLE hComm, OVERLAPPED *osReader);

static int initXBeeAndCom(void)
{
    char modeCmd[3] = {'+','+','+'};
    char baudRateAndStop[11] = {'A','T','B','D',0x07,0x0D,'A','T','C','N',0x0D};
    int step = 0;
    int tmp;



    if(hComm == NULL)
    {
        #ifdef XBEE_ERROR
        printf("error start XBee\n");
        #endif
        return 0;
    }

    tmp = SDL_GetTicks();
    while(SDL_GetTicks() - tmp < 1000)
    {
        Joystick();
        SDL_Delay(20);
    }

    WriteABuffer(modeCmd, 3, hComm, &Oo);

    #ifdef START_UP
    printf("Start 30/100, time = %6d\n", SDL_GetTicks());
    #endif

    //SDL_Delay(1200);

    tmp = SDL_GetTicks();
    while(step<1)
    {
        if(SDL_GetTicks() - tmp > 2000)
        {
            #ifdef XBEE_ERROR
            printf("error XBee conf time out\n");
            #endif
            return 0;
        }
        step += readXBee(hComm, &Oo);
        Joystick();
        SDL_Delay(20);
    }

    WriteABuffer(baudRateAndStop, 11, hComm, &Oo);

    #ifdef START_UP
    printf("Start 60/100, time = %6d\n", SDL_GetTicks());
    #endif

    //SDL_Delay(1500);

    tmp = SDL_GetTicks();
    while(step<3)
    {
        if(SDL_GetTicks() - tmp > 2000)
        {
            #ifdef XBEE_ERROR
            printf("error XBee conf time out\n");
            #endif
            return 0;
        }
        step += readXBee(hComm, &Oo);
        Joystick();
        SDL_Delay(20);
    }

    #ifdef START_UP
    printf("Start 90/100, time = %6d\n", SDL_GetTicks());
    #endif

    // CloseHandle(hComm);

    // hComm = link(CONF_UART_115200_BAUD);

    // if(hComm == NULL)
    // {
    //     #ifdef XBEE_ERROR
    //     printf("error start XBee\n");
    //     #endif
    //     return 0;
    // }

    return 1;
}


static void closeXBeeAndCom(void)
{
    char modeCmd[3] = {'+','+','+'};
    char baudRateAndStop[11] = {'A','T','B','D',0x03,0x0D,'A','T','C','N',0x0D};
    int step = 0;
    int tmp;

    if(hComm == NULL)
    {
        #ifdef XBEE_ERROR
        printf("error stop XBee\n");
        #endif
        return;
    }

    tmp = SDL_GetTicks();
    while(SDL_GetTicks() - tmp < 1200)
    {
        Joystick();
        SDL_Delay(20);
    }

    WriteABuffer(modeCmd, 3, hComm, &Oo);

    #ifdef START_UP
    printf("Stop 30/100, time = %6d\n", SDL_GetTicks());
    #endif

    //SDL_Delay(1200);

    tmp = SDL_GetTicks();
    while(step<1)
    {
        if(SDL_GetTicks() - tmp > 2000)
        {
            #ifdef XBEE_ERROR
            printf("error XBee conf time out\n");
            #endif
            return;
        }
        step += readXBee(hComm, &Oo);
        Joystick();
        SDL_Delay(20);
    }

    WriteABuffer(baudRateAndStop, 11, hComm, &Oo);

    #ifdef START_UP
    printf("Start 60/100, time = %6d\n", SDL_GetTicks());
    #endif

    //SDL_Delay(1500);

    tmp = SDL_GetTicks();
    while(step<3)
    {
        if(SDL_GetTicks() - tmp > 2000)
        {
            #ifdef XBEE_ERROR
            printf("error XBee conf time out\n");
            #endif
            return;
        }
        step += readXBee(hComm, &Oo);
        Joystick();
        SDL_Delay(20);
    }

    #ifdef START_UP
    printf("Start 90/100, time = %6d\n", SDL_GetTicks());
    #endif

    CloseHandle(hComm);

}




int readXBee(HANDLE hComm, OVERLAPPED *osReader)
{
    unsigned char pRxBuffer[20] = {0,};
    static unsigned char save[3] = {0,};
    static unsigned int saveLength = 0;
    int ret = 0;

    DWORD dwRead, i;

    // Issue read operation.
    if (!ReadFile(hComm, pRxBuffer, 100, &dwRead, osReader)) {
        if (GetLastError() != ERROR_IO_PENDING) {    // read not delayed?
        }
    } else if (dwRead) {
        // read completed immediately
        unsigned int rxIndice = 0;
        unsigned int rxLength = dwRead;
        while(rxLength)
        {
            if(rxLength + saveLength < 3)
            {
                save[saveLength] = pRxBuffer[rxIndice];
                #ifdef UART_READ_BRUT
                printf("read brut %x\n",pRxBuffer[rxIndice]);
                #endif
                saveLength ++;
                rxIndice ++;
                rxLength --;
            }
            else
            {
                while(saveLength < 3)
                {
                    save[saveLength] = pRxBuffer[rxIndice];
                    #ifdef UART_READ_BRUT
                    printf("read brut %x\n",pRxBuffer[rxIndice]);
                    #endif
                    saveLength ++;
                    rxIndice ++;
                    rxLength --;
                }
                if(save[0] == 0x4F && save[1] == 0x4B && save[2] == 0x0D)
                {
                    /* ok received from XBee */
                    ret ++;
                }
                else
                {
                    #ifdef UART_ERROR
                    printf("unknow XBee return : %x%x%x\n", save[0],save[1],save[2]);
                    #endif
                }
                saveLength = 0;
            }
        }
    }
    return ret;
}