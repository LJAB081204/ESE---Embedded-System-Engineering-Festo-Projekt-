/*
 * testing.h
 *
 *  Created on: 22.04.2026
 *      Author: tayla
 */

#ifndef TESTING_H_
#define TESTING_H_

#include "HAL.h"

class testing {
public:
	testing();
	//Evtl. ersetzen durch Methoden die assertions mit den registern nutzen
	void testSensors();
	void testMotor();
	void testLamps();
	void testButtons();

};





#endif /* TESTING_H_ */
