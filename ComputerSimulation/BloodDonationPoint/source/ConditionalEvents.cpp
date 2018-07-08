#include <iostream>
 
#include "ConditionalEvents.h"
#include "Hospital.h"
#include "Enums.h"
#include "Generators.h"

Generator* standard_order_generator = new Generator(9938305);
Generator* emergency_order_generator = new Generator(7927275);

//********************* Constructors *********************
EPatientService::EPatientService(Hospital* hospital)
{
	//std::cout << "EPatientService constructor call" << std::endl;
	pHospital_ = hospital;
}

EStandardOrder::EStandardOrder(Hospital* hospital) : Z_(1900), event_time_(0)
{
	//std::cout << "EStandardOrder constructor call" << std::endl;
	pHospital_ = hospital;
	blood_units_ = pHospital_->GetStandardOrderSize();
}
//	                                                      400
EEmergencyOrder::EEmergencyOrder(Hospital* hospital) : E_(400), W_(0.23), event_time_(0)
{
	//std::cout << "EEmergency constructor call" << std::endl;
	pHospital_ = hospital;
	blood_units_ = pHospital_->GetEmergencyOrderSize();
}

//********************* Methods *********************
void ConditionalEvent::Execute()
{
}

void EPatientService::Execute()
{
	//std::cout << "EPatientService::Execute()" << std::endl;

	//take needed blood units
	const int needed_blood = pHospital_->NeededBloodUnits();
	pHospital_->UseBlood(needed_blood);

	//delete first patient that is in queue
	pHospital_->DeletePatient();
}

void EStandardOrder::Execute()
{
	//std::cout << "EStandardOrder::Execute()" << std::endl;
	//in constructor set number of ordered blood units

	//plan delivery time	
	event_time_ = static_cast<int>(standard_order_generator->ExponentialDistribution(Z_));
	
	//set flag - order
	pHospital_->SetFlag(standard);
}

void EEmergencyOrder::Execute()
{
	//std::cout << "EEmergencyOrder::Execute()" << std::endl;
	//in constructor set number of ordered blood units

	//plan delivery time
	event_time_ = static_cast<int>(emergency_order_generator->NormalDistribution(E_,0.1));
	
	//set flag - emergency_order
	pHospital_->SetFlag(emergency);
}
