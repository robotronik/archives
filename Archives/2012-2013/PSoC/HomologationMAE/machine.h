/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#define NB_CMD 5

enum State { PRE_INIT, INIT, WORK, TEST_SHARP, STOP, PUITS };
typedef enum State State;

enum TypeCmd {ALPHA, DELTA};
typedef enum TypeCmd TypeCmd;

struct Depl 
{
	TypeCmd t;
	int val;
};
typedef struct Depl Depl;

void initTabDepl(void);
void machine(void);

//[] END OF FILE
