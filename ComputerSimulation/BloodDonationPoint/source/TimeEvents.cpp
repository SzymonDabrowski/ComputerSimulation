#include <random>
#include <iostream>

#include "Enums.h"
#include "Hospital.h"
#include "TimeEvents.h"
#include "Generators.h"

Generator* donor_generator = new Generator(550794902);
Generator* patient_generator = new Generator(1826);
Generator* A4_generator = new Generator(34109135);


//********************* Constructs *********************

TimeEvent::TimeEvent() : event_time(0) //first event time 
{
	//std::cout << "TimeEvent constructor call" << std::endl;
	time_of_expiration = 0;
}

//																			300
EDelivery::EDelivery(Hospital* hospital, int delivery_time, int type) : T1_(700)
{
	//std::cout << "EDelivery constructor call" << std::endl;

	type_ = type;
	pHospital_ = hospital;
	switch (type)
	{
	case standard:
		blood_units_ = hospital->GetStandardOrderSize();
		break;
	case emergency:
		blood_units_ = hospital->GetEmergencyOrderSize();
		break;
	}
	event_time = delivery_time;
	event_name = blood_delivery; 

	time_of_expiration = T1_;
}

EDonorArrival::EDonorArrival(Hospital *hospital) : 	T2_(1200), L_(850)
{
	//std::cout << "EDonorArrival constructor call" << std::endl;

	pHospital_ = hospital;
	new_unit_ = new BloodUnit(T2_);
	event_name = donor_arrived;

	time_of_expiration = T2_;
}

EPatientArrival::EPatientArrival(Hospital* hospital) : W_(0.23), P_(200)
{
	//std::cout << "EPatientArrival constructor call" << std::endl;

	pHospital_ = hospital;
	event_name = patient_arrived;
}

EUtilization::EUtilization(Hospital * hospital)
{
	//std::cout << "EUtilization constructor call" << std::endl;

	pHospital_ = hospital;
	event_name = utilization;
}

EA4Utilization::EA4Utilization(Hospital * hospital)
{
	//std::cout << "E4AUtilization constructor call" << std::endl;

	pHospital_ = hospital;
	event_name = A4;
	event_time = 0; // if conditions are OK, then just do Execute()
}


//********************* Methods *********************
const int& TimeEvent::GetEventTime() const
{
	return event_time;
}

const int& TimeEvent::GetEventName() const
{
	return event_name;
}

void TimeEvent::SetEventTime(int time)
{
	event_time = time;
}

void TimeEvent::UpdateTime(int system_time)
{
	event_time -= system_time;
}

void TimeEvent::Execute()
{
}

void EDelivery::Execute()
{
	//std::cout << "EDelivery::Execute()" << std::endl;
	pHospital_->AddOrderedBlood(blood_units_, T1_);
	pHospital_->ClearFlag(type_); 
}

void EDonorArrival::Execute()
{
	//std::cout << "EDonorArrival::Execute()" << std::endl;

	pHospital_->AddDonorsBlood(new_unit_);

	// plan next donors arrival
	event_time = donor_generator->ExponentialDistribution(L_);
}

void EPatientArrival::Execute()
{
	//std::cout << "EPatientArrival::Execute()" << std::endl;

	//liczba potrzebnych jednostek jest losowana
	//w konstruktorze pacjenta
	Patient* patient = new Patient();

	pHospital_->AddPatientToQueue(patient);

	event_time = static_cast<int>(patient_generator->ExponentialDistribution(P_));
}

void EUtilization::Execute()
{
	//std::cout << "EUtilization::Execute()" << std::endl;
	
	//delete blood units that are out of date
	pHospital_->DeleteBlood();
}

void EA4Utilization::Execute()
{
	//std::cout << "EA4Utilization::Execute()" << std::endl;

	//Generator generator;
	//int blood_to_delete = static_cast<int>(generator.UniformDistribution(5,10));

	int blood_to_delete = event_time = static_cast<int>(patient_generator->UniformDistribution(5,10));

	//delete blood units that are out of date
	pHospital_->DeleteBlood(blood_to_delete);
}