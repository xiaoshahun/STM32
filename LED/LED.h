#ifndef __LED_H
#define __LED_H

#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

//LED为反向输出


void LED_init(void);
void Light(void);


#endif

