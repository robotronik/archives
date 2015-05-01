;extern long asmFIR(int * coef_fir, int * tab_adc_brut , int start_coef, int NB_COEF_FIR);
;
;W0 : in coef_fir address yMem
;W1 : in tab_adc_brut_adress xMem
;W2 : in start coef
;W3 : in NB_COEF_FIR -1
;
;W0 : out resultatH/L   TODO
;W1 : out resultatH/L
;
;W0-W7 : not saved
;W8-W15 : saved - used - restored
;
;
;CYCLES : environ 350 (300MAC + le reste)

;TODO : si start coef != 0 alors +dernier coef  -premier coef



    .global _asmFIR
;    .EXTERN NB_COEF_FIR
;    .section .fir,code


_asmFIR:
    push W8 ; TODO non push/pop les reg non utilisés
;    push W9
    push W10
;    push W11
;    push W12
;    push W13
;    push W14
;    push W15

    MOV W0,W10   ;W10 <- yMem (coef fir)
    ADD W1,W2,W8 ;W8 <-  xMem (&tabadc[startcoef])
    ADD W8,W2,W8 ;deux add car startcoef=1 inc pointeur de 2
    CLR A, [W8]+=2 , W4 , [W10]+=2 , W5
    REPEAT W3
    MAC W4*W5 , A, [W8]+=2 , W4 , [W10]+=2 , W5
    MAC W4*W5 , A  ;dernier MAC sans prefrech
    ;SAC A,#0, W0 ; store result in W0, no shift
    MOV ACCAH,W1
    MOV ACCAL,W0  ;W1 <- 0           TODO vérifier quel poids fort ou faible sur W0

    pop W8
;    pop W9
    pop W10
;    pop W11
;    pop W12
;    pop W13
;    pop W14
;    pop W15
    nop
    nop

    return

    .end
