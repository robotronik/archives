L'execution sur PSoC a �t� laiss�e � 24 MHz. Cette fr�quence d'execution ne suffie pas � traiter les hautes fr�quences du signal venant du convertisseur lumi�re->fr�quence.

Le projet Final_frequence_mix_ssInt est un suiveur de ligne.
Le projet 2_Capteurs est � passer sur PIC � 80 MHz. 1 seul capteur par PIC ? La fonction convert_RGB_to_HSL ne devrait pas �tre appel�e dans l'interruption mais lorsque les valeurs HSL sont n�cessaires.

Les fichiers d'interruptions ne s'affichent pas � l'ouverture du workspace mais ils sont bien pr�sent dans "\Generated_Source\PSoC5".
Le code le plus important est ligne 143 de "\Generated_Source\PSoC5\isr_LED.c". Il est execut� p�riodiquement.
La fonction convert_RGB_to_HSL dans capteur.c est � configurer (d�commenter) en fonction du besoin de d�tection. Les seuils peuvent �tre rer�gl�s.