 #include "init.h"
 #include "pwm.h"
 #include "timer.h"
 #include "main.h"
int main()
{
	initClk();
	initPWM();
	while(1)
	{
		
		for(i=1; i <=3 ; i++)
		{
			sendXBEE(i);
			activePWM(i);
			waitMs(75);//On attend que l'onde ai parcouru toute la tombe + reflexion
		}
		wait(1000);//Attente de 1s pour la prochaine salve
		
		
	}
	

}


