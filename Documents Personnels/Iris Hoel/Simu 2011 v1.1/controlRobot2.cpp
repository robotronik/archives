
# include "main.hpp"


extern Inst instructionRobot2;



void Joystick(){
//log("joy");
SDL_Event event;
SDL_PollEvent(&event);
switch(event.type)
{
    case SDL_JOYBUTTONDOWN:

        break;
    case SDL_JOYAXISMOTION:
        if (event.jaxis.axis == 2)
        instructionRobot2.vitesse = ((-event.jaxis.value)+32768.0)/200000.0;

        else if (event.jaxis.axis == 0 && event.jaxis.value < -3000){ /* Vers la gauche */
        instructionRobot2.signe =1;
        instructionRobot2.type = TOURNER;
        }
        else if (event.jaxis.axis == 0 && event.jaxis.value > 3000){
        instructionRobot2.signe =-1;
        instructionRobot2.type = TOURNER;
        }
        else if (event.jaxis.axis == 1 && event.jaxis.value < -10000){
        instructionRobot2.signe =1;
        instructionRobot2.type = AVANCER;
        }
        // avant
        else if (event.jaxis.axis == 1 && event.jaxis.value > 10000){
        instructionRobot2.signe = -1;
        instructionRobot2.type = AVANCER;
        } /* Vers le bas */
        else{
        instructionRobot2.vitesse = 0;
        }



    break;
}


}






