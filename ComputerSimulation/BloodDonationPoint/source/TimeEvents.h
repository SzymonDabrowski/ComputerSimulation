#pragma once
 
#include "Hospital.h"
#include "BloodUnit.h"

class Hospital; // without this -> error

//********************* Base class *********************
class TimeEvent
{
public:
	const int &GetEventTime() const;
	const int &GetEventName() const;
	void SetEventTime(int time);
	void UpdateTime(int system_time);

	const int& GetTimeOfExpiration() { return time_of_expiration; }

	virtual void Execute();

	TimeEvent();
	~TimeEvent() = default;

protected:
	int event_time;
	int event_name;

	int time_of_expiration;
};

//******************* Derived classes *******************
class EDelivery : public TimeEvent
{
public:
	const int& GetBloodUnits() const { return blood_units_; }

	void Execute() override;

	EDelivery(Hospital* hospital, int delivery_time, int type);
	~EDelivery() = default;

private:
	int type_; //type of order
	int blood_units_;
	const int T1_;
	Hospital* pHospital_;
};

class EDonorArrival : public TimeEvent
{
public:
	void Execute() override;

	EDonorArrival(Hospital* hospital);
	~EDonorArrival() = default;

private:
	const int T2_;
	const int L_;
	Hospital *pHospital_;
	BloodUnit *new_unit_;
};

class EPatientArrival : public TimeEvent
{
public:
	void Execute() override;

	explicit EPatientArrival(Hospital* hospital);
	~EPatientArrival() = default;

private:
	Hospital * pHospital_;
	const double W_; //mean 1/W_ in geom. distrib. - to draw blood units
	const int P_; //mean P in exp. distrib. - to draw next events time
};

class EUtilization : public TimeEvent
{
public:
	void Execute() override;

	explicit EUtilization(Hospital* hospital);
	~EUtilization() = default;

private:
	Hospital * pHospital_;
};

class EA4Utilization : public TimeEvent
{
public:
	void Execute() override;

	explicit EA4Utilization(Hospital* hospital);
	~EA4Utilization() = default;

private:
	Hospital * pHospital_;
};