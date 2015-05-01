#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>


unsigned char load = 0x0;
//unsigned char recu;
unsigned  recu;
int nbBuffer = 0;
unsigned char toSend;

 
void setup(){
  Serial.begin(115200);
   
  Mirf.cePin = 8; // CE sur D8
  Mirf.csnPin = 7; // CSN sur D7
  Mirf.spi = &MirfHardwareSpi; // On veut utiliser le port SPI hardware
  Mirf.init(); // Tout est bon ? Ok let's go !
 
  Mirf.channel = 42; // On va utiliser le canal 0 pour communiquer (128 canaux disponible, de 0 à 127)
  //Mirf.payload = sizeof(unsigned char); // = 4, ici il faut déclarer la taille du "payload" soit du message qu'on va transmettre, au max 32 octets
  Mirf.payload = sizeof(unsigned char);
  Mirf.config(); // Tout est bon ? Ok let's go !
   
  Mirf.setTADDR((byte *)"quad1"); // Le 1er module va envoyer ses info au 2eme module
  Mirf.setRADDR((byte *)"telec"); // On définit ici l'adresse du 1er module
   
  Serial.println("Go !");
  delay(15);
  Serial.print("status : "); 
  Serial.println(Mirf.getStatus());
  Mirf.send((byte *)&load); //on envoie 0
  while(Mirf.isSending());
  Serial.print(load, HEX);
  Serial.println("");
  delay(10);
}
 
void loop(){
    //unsigned long time = millis(); // On stock le temps actuelle retourné par millis() dans time
 
  nbBuffer = Serial.available();
  if (nbBuffer > 0) { // si données disponibles sur le port série
    do {
          load = Serial.read();
    } while (--nbBuffer);
    Serial.flush();
    
       
    Mirf.send((byte *)&load); 
    while(Mirf.isSending()); // On boucle (attend) tant que le message n'as pas était envoyé
     
    //Serial.print("send ... ");
    Serial.print(load, HEX);
    Serial.println("");
    delay(10);
  }
  
  /* reception */
  if (!Mirf.isSending() && Mirf.dataReady())
  {
      Mirf.getData((byte *) &recu);
      Serial.print("r:");
      Serial.println(recu);
  }
   
   
  delay(100);
} 
