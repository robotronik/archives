#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
 
void setup(){
  Serial.begin(9600);
   
  Mirf.cePin = 8; // CE sur D8
  Mirf.csnPin = 7; // CSN sur D7
  Mirf.spi = &MirfHardwareSpi; // On veut utiliser le port SPI hardware
  Mirf.init(); // Tout est bon ? Ok let's go !
 
  Mirf.channel = 0; // On va utiliser le canal 0 pour communiquer (128 canaux disponible, de 0 à 127)
  Mirf.payload = sizeof(unsigned long); // = 4, ici il faut déclarer la taille du "payload" soit du message qu'on va transmettre, au max 32 octets
  Mirf.config(); // Tout est bon ? Ok let's go !
   
  Mirf.setTADDR((byte *)"nrf02"); // Le 1er module va envoyer ses info au 2eme module
  Mirf.setRADDR((byte *)"nrf01"); // On définit ici l'adresse du 1er module
   
  Serial.println("Go !");
}
 
void loop(){
  //unsigned long time = millis(); // On stock le temps actuelle retourné par millis() dans time
  unsigned long load = 0xABCDEF12;
  unsigned long recu;
   
  Mirf.send((byte *)&load); 
   
  while(Mirf.isSending()); // On boucle (attend) tant que le message n'as pas était envoyé
   
  Serial.print("send ... ");
  Serial.print(load, HEX);
  Serial.println("");
  delay(10);
  
  if (!Mirf.isSending() && Mirf.dataReady())
  {
      Mirf.getData((byte *) &recu);
      Serial.print("received : ");
      Serial.print(recu, HEX);
      Serial.println("");
  }
   
  /*while(!Mirf.dataReady()){ // On attend de recevoir quelque chose
    if ( ( millis() - time ) > 1000 ) { // Si on attend depuis plus d'une seconde
      Serial.println("=("); // C'est le drame ...
      return;
    }
  }
   
  Mirf.getData((byte *) &time); // On récupére le message recu
   
  Serial.print(millis() - time); // On affiche le temps de latence
  Serial.println("ms");*/
   
  delay(1000);
} 
