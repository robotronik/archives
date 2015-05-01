import pygame
from pygame.locals import *
import serial
from ctypes import *

HAUTEUR = 300
LARGEUR = 500

DEBUT_PITCH = 100
L_BC_PITCH = 80
 
pygame.init()
fenetre = pygame.display.set_mode((LARGEUR,HAUTEUR))
fond = pygame.Rect(0, 0, LARGEUR, HAUTEUR)
cl = (0xb0, 0xb0, 0xb0)
myfont = pygame.font.SysFont("monospace", 15)

gaucheVertical = 0
gaucheHorizontal = 0
droiteVertical = 0
droiteHorizontal = 0
precEnvoi = 0


def lefond():
    pygame.draw.rect(fenetre, (0,0,0) , fond)
    #contour gaz
    contour = pygame.Rect(8, HAUTEUR/2+4, 30+5, -107);    
    pygame.draw.rect(fenetre, (0xff,0x0,0x0), contour)
    contour2 = pygame.Rect(10, HAUTEUR/2, 30, -100);    
    pygame.draw.rect(fenetre, (0x00,0x0,0x0), contour2)

    #contour pitch
    barre =  pygame.Rect(DEBUT_PITCH, HAUTEUR/2-1-40, L_BC_PITCH, 2);
    pygame.draw.rect(fenetre, (0xff,0x0,0x0), barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-10)/2, HAUTEUR/2-7-40, 10, 2);
    pygame.draw.rect(fenetre,cl, barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-45)/2, HAUTEUR/2-15-40, 45, 2);
    pygame.draw.rect(fenetre,cl, barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-10)/2, HAUTEUR/2-22-40, 10, 2);
    pygame.draw.rect(fenetre,cl, barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-60)/2, HAUTEUR/2-30-40, 60, 2);
    pygame.draw.rect(fenetre,cl, barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-10)/2, HAUTEUR/2-37-40, 10, 2);
    pygame.draw.rect(fenetre,cl, barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-75)/2, HAUTEUR/2-45-40, 75, 2);
    pygame.draw.rect(fenetre,cl, barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-10)/2, HAUTEUR/2+7-40, 10, 2);
    pygame.draw.rect(fenetre,cl, barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-45)/2, HAUTEUR/2+15-40, 45, 2);
    pygame.draw.rect(fenetre,cl, barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-10)/2, HAUTEUR/2+22-40, 10, 2);
    pygame.draw.rect(fenetre,cl, barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-60)/2, HAUTEUR/2+30-40, 60, 2);
    pygame.draw.rect(fenetre,cl, barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-10)/2, HAUTEUR/2+37-40, 10, 2);
    pygame.draw.rect(fenetre,cl, barre)
    barre =  pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-75)/2, HAUTEUR/2+45-40, 75, 2);
    pygame.draw.rect(fenetre,cl, barre)
    #pygame.display.flip();

#On compte les joysticks
nb_joysticks = pygame.joystick.get_count()
print 'nb de joystick : ' +  str(pygame.joystick.get_count());
if nb_joysticks > 0:
    manette = pygame.joystick.Joystick(0)
    
    manette.init()
    # fond tout noir 
    pygame.draw.rect(fenetre, (0,0,0) , fond)

    # bar de gaz
    rect = pygame.Rect(10, HAUTEUR/2, 30, 0)
    pygame.draw.rect(fenetre, cl, rect)

    lefond()
    pygame.display.flip();

    #liaison serie
    try:
        ser = serial.Serial('/dev/ttyACM0', 115200)
    except serial.SerialException :
        print 'Erreur liasion serie'
        ser = 0
 
    #On compte les boutons
    nb_boutons = manette.get_numbuttons()
    if nb_boutons >= 4:
     
        continuer = 1
        while continuer:
            # fond tout noir 
            for event in pygame.event.get():
                #pygame.draw.rect(fenetre, (0,0,0) , fond)
                #pygame.display.flip();
                if event.type == QUIT:
                    continuer = 0
                elif event.type == JOYAXISMOTION and event.axis == 0:
                    print 'gaucheHorizontal:' + str(event.value);
                elif event.type == JOYAXISMOTION and event.axis == 1:
                    gaucheVertical = event.value;
                    print 'gaucheVertical:' + str(event.value);
                    if gaucheVertical > 0:
                        gaucheVertical = 0
                elif event.type == JOYAXISMOTION and event.axis == 2:
                    print 'droiteHorizontal:' + str(event.value);
                elif event.type == JOYAXISMOTION and event.axis == 3:
                    print 'droiteVertical:' + str(event.value);
                    droiteVertical = event.value;
                if event.type == JOYBUTTONDOWN and event.button == 0:
                    print("Croix")
                elif  event.type == JOYBUTTONDOWN and event.button == 1:
                    print("Carre")
                elif  event.type == JOYBUTTONDOWN and event.button == 2:
                    print("Rond")
                elif  event.type == JOYBUTTONDOWN and event.button == 3:
                    print("Triangle")

                # envoie des donnees a l'arduino 
                if ser != 0:
                    envoi = chr(int(-gaucheVertical*255))
                    if envoi != precEnvoi:
                        ser.write(envoi)
                        precEnvoi = envoi
                #affichage
                lefond()
                
                #Gaz
                rect = pygame.Rect(10, HAUTEUR/2, 30,gaucheVertical*100)
                pygame.draw.rect(fenetre, cl, rect)
                label = myfont.render(str(int(-gaucheVertical*100)) +"%", 1, (255,255,0))
                fenetre.blit(label, (12, 168))
                label2 = myfont.render(str(hex((int(-gaucheVertical*255)))), 1, (255,255,0))
                fenetre.blit(label2, (12, 190))

                #pitch
                rect = pygame.Rect(DEBUT_PITCH+(L_BC_PITCH-78)/2, HAUTEUR/2-40+droiteVertical*50, 78,2)
                pygame.draw.rect(fenetre,(0xff,0xff,0x0), rect)
                pygame.display.flip();


            

