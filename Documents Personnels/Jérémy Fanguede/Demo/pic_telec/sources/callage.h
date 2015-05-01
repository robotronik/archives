#define CALLAGE CALLE_G:										\
		case CALLE_H:											\
		case CALLE_D:											\
			if (vitDelta) commandeCourante += 3;				\
			motors_set_speed(-min_speed-100, -min_speed-100);	\
			continue;											\
																\
		case CALLE_G2:											\
			if (!vitDelta) {									\
				/*posX = PROFONDEUR/2;*/						\
				posAlpha = 0;									\
				motors_stop();									\
				commandeCourante = FREINAGE;					\
			}													\
			continue;											\
																\
		case CALLE_H2:											\
			if (!vitDelta) {									\
				/*posY = PROFONDEUR/2;*/						\
				posAlpha = TOUR*3 >> 2;							\
				motors_stop();									\
				commandeCourante = FREINAGE;					\
			}													\
			continue;											\
																\
		case CALLE_D2:											\
			if (!vitDelta) {									\
				/*posX = 0xFFFFFFFF - PROFONDEUR/2;*/			\
				posAlpha = TOUR >> 1;							\
				motors_stop();									\
				commandeCourante = FREINAGE;					\
			}													\
			continue;
