#include <iostream>
#include <random>
#include "Generators.h"

#include "Patient.h"

Generator* needed_blood_generator = new Generator(89734);

Patient::Patient() : W_(0.23) //'W_' also in C.Events [may put in Hospital and give it in contructors]
{
	//std::cout << "Patient constructor call" << std::endl;

	// draws how many blood units will patient need
	blood_units_needed_ = needed_blood_generator->GeometricDistribution(1/W_);
} 