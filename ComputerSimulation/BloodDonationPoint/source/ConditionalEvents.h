#pragma once
 
#include "Hospital.h"

//TODO: think about not using Hospital* int constructors
//		better use specific attrib, like blood_depot_*
//		hospital should not be global!

//********************* Base class *********************
class ConditionalEvent
{
public:
	virtual void Execute();

	ConditionalEvent() = default;
	virtual ~ConditionalEvent() = default;
};

//******************* Derived classes *******************
class EPatientService : public ConditionalEvent
{
public:
	void Execute() override;

	EPatientService(Hospital* hospital);
	~EPatientService() = default;

private:
	Hospital * pHospital_;
};

class EStandardOrder : public ConditionalEvent
{
public:
	void Execute() override;

	int GetEventTime() { return event_time_; }

	EStandardOrder(Hospital* hospital);
	~EStandardOrder() = default;

private:
	Hospital * pHospital_;
	const int Z_;
	int blood_units_;
	int event_time_;
};

class EEmergencyOrder : public ConditionalEvent
{
public:
	void Execute() override;

	int GetEventTime() { return event_time_; }

	EEmergencyOrder(Hospital* hospital);
	~EEmergencyOrder() = default;

private:
	Hospital * pHospital_;
	const int E_;
	const double W_; // also in Patient
	int blood_units_;
	int event_time_;
};
