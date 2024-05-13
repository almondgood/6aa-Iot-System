/*
 * dht11.h
 *
 *  Created on: Apr 18, 2024
 *      Author: USER
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "stm32f4xx_hal.h"

#define DHT11_PORT GPIOC
#define DHT11_PIN GPIO_PIN_3



uint8_t DHT11_Start(void);


/*
 * 	      RHI = DHT11_Read(); // Relative humidity integral
 *	      RHD = DHT11_Read(); // Relative humidity decimal
 *	      TCI = DHT11_Read(); // Celsius integral
 *	      TCD = DHT11_Read(); // Celsius decimal
 *	      SUM = DHT11_Read(); // Check sum
 */
uint8_t DHT11_Read (void);


#endif /* INC_DHT11_H_ */
