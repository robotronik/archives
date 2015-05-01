# MPLAB IDE generated this makefile for use with GNU make.
# Project: projet.mcp
# Date: Thu Apr 19 19:01:33 2012

AS = pic30-as.exe
CC = pic30-gcc.exe
LD = pic30-ld.exe
AR = pic30-ar.exe
HX = pic30-bin2hex.exe
RM = rm

projet.hex : projet.cof
	$(HX) "projet.cof"

projet.cof : main.o motors.o message.o init.o asser.o conversion.o debug.o uart.o myMath.o
	$(CC) -mcpu=33FJ128MC802 "main.o" "motors.o" "message.o" "init.o" "asser.o" "conversion.o" "debug.o" "uart.o" "myMath.o" -o"projet.cof" -Wl,-Tp33FJ128MC802.gld,--defsym=__MPLAB_BUILD=1,-Map="projet.map",--report-mem

main.o : sources/motors.h sources/message.h sources/uart.h sources/init.h sources/conversion.h sources/debug.h sources/main.h sources/asser.h c:/program\ files\ (x86)/microchip/mplabc30/v3.25/support/dsPIC33F/h/p33FJ128MC802.h sources/main.h sources/main.c
	$(CC) -mcpu=33FJ128MC802 -x c -c "sources\main.c" -o"main.o" -g -Wall

motors.o : sources/motors.h sources/uart.h sources/init.h sources/conversion.h sources/debug.h sources/main.h sources/asser.h c:/program\ files\ (x86)/microchip/mplabc30/v3.25/support/dsPIC33F/h/p33FJ128MC802.h sources/main.h sources/motors.c
	$(CC) -mcpu=33FJ128MC802 -x c -c "sources\motors.c" -o"motors.o" -g -Wall

message.o : sources/motors.h sources/asser.h sources/message.h sources/uart.h sources/init.h sources/conversion.h sources/debug.h sources/main.h sources/asser.h c:/program\ files\ (x86)/microchip/mplabc30/v3.25/support/dsPIC33F/h/p33FJ128MC802.h sources/main.h sources/message.c
	$(CC) -mcpu=33FJ128MC802 -x c -c "sources\message.c" -o"message.o" -g -Wall

init.o : sources/motors.h sources/uart.h sources/init.h sources/conversion.h sources/debug.h sources/main.h sources/asser.h c:/program\ files\ (x86)/microchip/mplabc30/v3.25/support/dsPIC33F/h/p33FJ128MC802.h sources/main.h sources/init.c
	$(CC) -mcpu=33FJ128MC802 -x c -c "sources\init.c" -o"init.o" -g -Wall

asser.o : sources/message.h sources/motors.h sources/myMath.h sources/uart.h sources/init.h sources/conversion.h sources/debug.h sources/main.h sources/asser.h c:/program\ files\ (x86)/microchip/mplabc30/v3.25/support/dsPIC33F/h/p33FJ128MC802.h sources/main.h sources/asser.c
	$(CC) -mcpu=33FJ128MC802 -x c -c "sources\asser.c" -o"asser.o" -g -Wall

conversion.o : sources/uart.h sources/init.h sources/conversion.h sources/debug.h sources/main.h sources/asser.h c:/program\ files\ (x86)/microchip/mplabc30/v3.25/support/dsPIC33F/h/p33FJ128MC802.h sources/main.h sources/conversion.c
	$(CC) -mcpu=33FJ128MC802 -x c -c "sources\conversion.c" -o"conversion.o" -g -Wall

debug.o : sources/uart.h sources/init.h sources/conversion.h sources/debug.h sources/main.h sources/asser.h c:/program\ files\ (x86)/microchip/mplabc30/v3.25/support/dsPIC33F/h/p33FJ128MC802.h sources/main.h sources/debug.c
	$(CC) -mcpu=33FJ128MC802 -x c -c "sources\debug.c" -o"debug.o" -g -Wall

uart.o : c:/program\ files\ (x86)/microchip/mplabc30/v3.25/include/stdarg.h c:/program\ files\ (x86)/microchip/mplabc30/v3.25/include/stddef.h c:/program\ files\ (x86)/microchip/mplabc30/v3.25/include/stdio.h sources/message.h sources/uart.h sources/init.h sources/conversion.h sources/debug.h sources/main.h sources/asser.h c:/program\ files\ (x86)/microchip/mplabc30/v3.25/support/dsPIC33F/h/p33FJ128MC802.h sources/main.h sources/uart.c
	$(CC) -mcpu=33FJ128MC802 -x c -c "sources\uart.c" -o"uart.o" -g -Wall

myMath.o : sources/uart.h sources/init.h sources/conversion.h sources/debug.h sources/asser.h c:/program\ files\ (x86)/microchip/mplabc30/v3.25/support/dsPIC33F/h/p33FJ128MC802.h sources/main.h sources/asser.h sources/myMath.c
	$(CC) -mcpu=33FJ128MC802 -x c -c "sources\myMath.c" -o"myMath.o" -g -Wall

clean : 
	$(RM) "main.o" "motors.o" "message.o" "init.o" "asser.o" "conversion.o" "debug.o" "uart.o" "myMath.o" "projet.cof" "projet.hex"

