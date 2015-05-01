#ifndef MESSAGES_H_INCLUDED
#define MESSAGES_H_INCLUDED

/*
    Fichier contenant les messages que peuvent s'envoyer les Engine entre eux
*/

// Types
#define MSG_CALC_PT   0x03
#define MSG_PT_ROUGE  0x04  // int* en alloc pour le data
#define MSG_PT_VIOLET 0x05  //     Idem

#define MSG_ONE_PLAYER 0x06
#define MSG_TWO_PLAYER 0x07

#define MSG_CMPT_HR_A 0x08 // int* en alloc pour le data
#define MSG_CMPT_HR_B 0x08 // int* en alloc pour le data
#define MSG_CMPT_HR_C 0x09 // int* en alloc pour le data
#define MSG_CMPT_HR_D 0x10 // int* en alloc pour le data

#define MSG_CMPT_RG_A 0x11 // idem, -1 : rien
#define MSG_CMPT_RG_B 0x12 // idem
#define MSG_CMPT_RG_C 0x13

#define MSG_CMPT_VI_A 0x14 // idem
#define MSG_CMPT_VI_B 0x15 // idem
#define MSG_CMPT_VI_C 0x16 // idem

#endif // MESSAGES_H_INCLUDED
