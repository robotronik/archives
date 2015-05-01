#define CALLAGE CALLE_G:																			\
		case CALLE_H:																				\
		case CALLE_D:																				\
			/* Attend le démarrage */																\
			if (vitDelta < 0) commandeCourante += (int)CALLE_G2 - CALLE_G;							\
			motors_set_speed(-min_speed-MAX_SPEED/10, -min_speed-MAX_SPEED/10);						\
			continue;																				\
																									\
		case CALLE_G2:																				\
			if (!vitDelta) {																		\
				posX.all = (long long)0xFFFFFFFF * (3000 - LONG_ARRIERE) / 3000;					\
				unsigned short alphaInitPrec = alphaInit;											\
				alphaInit = (TOUR + alphaInit - posAlpha) % TOUR;									\
				posAlphaPrec += (TOUR + alphaInit - alphaInitPrec) % TOUR;							\
				motors_stop();																		\
				commandeCourante = FREINAGE;														\
				UART_putc(DONE);																	\
			}																						\
			continue;																				\
																									\
		case CALLE_H2:																				\
			if (!vitDelta) {																		\
				posY.all = (long long)0xFFFFFFFF * LONG_ARRIERE / 3000;								\
				unsigned short alphaInitPrec = alphaInit;											\
				alphaInit = (((unsigned long) TOUR*3 >> 2) + TOUR + alphaInit - posAlpha) % TOUR;	\
				posAlphaPrec += (TOUR + alphaInit - alphaInitPrec) % TOUR;							\
				motors_stop();																		\
				commandeCourante = FREINAGE;														\
				UART_putc(DONE);																	\
			}																						\
			continue;																				\
																									\
		case CALLE_D2:																				\
			if (!vitDelta) {																		\
				posX.all = (long long)0xFFFFFFFF * LONG_ARRIERE / 3000;								\
				unsigned short alphaInitPrec = alphaInit;											\
				alphaInit = ((unsigned long) (TOUR >> 1) + TOUR + alphaInit - posAlpha) % TOUR;		\
				posAlphaPrec += (TOUR + alphaInit - alphaInitPrec) % TOUR;							\
				motors_stop();																		\
				commandeCourante = FREINAGE;														\
				UART_putc(DONE);																	\
			}																						\
			continue;
