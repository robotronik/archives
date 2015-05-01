L'execution sur PSoC a été laissée à 24 MHz. Cette fréquence d'execution ne suffie pas à traiter les hautes fréquences du signal venant du convertisseur lumière->fréquence.

Le projet Final_frequence_mix_ssInt est un suiveur de ligne.
Le projet 2_Capteurs est à passer sur PIC à 80 MHz. 1 seul capteur par PIC ? La fonction convert_RGB_to_HSL ne devrait pas être appelée dans l'interruption mais lorsque les valeurs HSL sont nécessaires.

Les fichiers d'interruptions ne s'affichent pas à l'ouverture du workspace mais ils sont bien présent dans "\Generated_Source\PSoC5".
Le code le plus important est ligne 143 de "\Generated_Source\PSoC5\isr_LED.c". Il est executé périodiquement.
La fonction convert_RGB_to_HSL dans capteur.c est à configurer (décommenter) en fonction du besoin de détection. Les seuils peuvent être reréglés.