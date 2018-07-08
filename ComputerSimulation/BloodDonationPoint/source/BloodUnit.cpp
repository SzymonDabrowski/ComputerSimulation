#include <iostream>
 
#include "BloodUnit.h"

int counter_blood_units = 0;

BloodUnit::BloodUnit(int T)
{
	//std::cout << "BloodUnit constructor call" << std::endl;
	time_to_expiration_ = T;
	counter_blood_units++;
}