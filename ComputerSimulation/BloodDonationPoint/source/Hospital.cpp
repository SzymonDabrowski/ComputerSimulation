#include <queue>
#include <vector>
#include <iostream>
 
#include "Hospital.h"
#include "Patient.h"
#include "BloodUnit.h"
#include "Enums.h"


struct sorting
{
	inline bool operator() (const BloodUnit* struct1, const BloodUnit* struct2)
	{
		return (struct1->GetTimeToExpiration() > struct2->GetTimeToExpiration());
	}
};


void Hospital::OrderInfo()
{
	if (flag_standard_order_ && flag_emergency_order_)
	{
		std::cout << "standard and emergency";
		return;
	}
	else if (flag_standard_order_)
	{
		std::cout << "standard";
		return;
	}
	else if (flag_emergency_order_)
	{
		std::cout << "emergency";
		return;
	}
	else
	{
		std::cout << "none";
		return;
	}
}

void Hospital::Time(int delta_time)
{
	for (int i = 0; i < blood_depot_.size(); ++i)
	{
		int time = blood_depot_[i]->GetTimeToExpiration();
		blood_depot_[i]->SetTimeToExpiration(time - delta_time);
	}
}

//*********************** Methods ***********************

bool Hospital::FlagInfo(int type)
{
	switch (type)
	{
	case standard:
		return flag_standard_order_;
	default: //case emergency:
		return flag_emergency_order_;
	}
}

void Hospital::SetFlag(int type)
{
	switch (type)
	{
	case standard:
		flag_standard_order_ = true;
		break;
	default: //case emergency:
		flag_emergency_order_ = true;
	}
}

void Hospital::ClearFlag(int type)
{
	switch (type)
	{
	case standard:
		flag_standard_order_ = false;
		break;
	default: //case emergency:
		flag_emergency_order_ = false;
	}
}

const int& Hospital::GetStandardOrderSize() const
{
	return N_;
}

const int& Hospital::GetEmergencyOrderSize() const
{
	return Q_;
}

int Hospital::GetBloodDepotSize() const
{
	return static_cast<int>(blood_depot_.size());
}

int Hospital::GetQueueSize() const
{
	return static_cast<int>(patients_queue_.size());
}

bool Hospital::IsEnoughBlood() const
{
	return static_cast<int>(blood_depot_.size()) > R_;
}

bool Hospital::IsQueueEmpty() const
{
	return patients_queue_.empty();
}

int Hospital::NeededBloodUnits()
{
	return patients_queue_.front()->GetBloodUnitsNeeded();
}

int Hospital::GetTU()
{
	return TU_;
}

int Hospital::GetTB()
{
	return TB_;
}

void Hospital::DeletePatient()

{
	Patient *to_delete = patients_queue_.front();
	delete to_delete;
	patients_queue_.pop();
}

void Hospital::AddPatientToQueue(Patient* patient)
{
	patients_queue_.push(patient);
}

void Hospital::UseBlood(const int N)
{
	for (int i = 0; i < N; ++i)
	{
		BloodUnit *to_delete = blood_depot_.back();
		delete to_delete;
		blood_depot_.pop_back();
	}
}


int counter_utilized_blood = 0;

//TODO: Write this to delete single blood unit maybe?
void Hospital::DeleteBlood()
{
	std::sort(blood_depot_.begin(), blood_depot_.end(), sorting());

	if (this->GetBloodDepotSize() == 0)
		return;

	//deletes blood that is out of date
	for (int i = 0; i < GetBloodDepotSize() - 1; ++i)
	{
		//queue must be sorted
		const int is_now = blood_depot_.back()->GetTimeToExpiration();
		if (is_now == 0)
		{
			BloodUnit *to_delete = blood_depot_.back();
			delete to_delete;
			blood_depot_.pop_back();
			counter_utilized_blood++;
		}
		else
			break;
	}
}

//may have errors
//if so, check if list is bigger than N elements
void Hospital::DeleteBlood(int N)
{
	std::sort(blood_depot_.begin(), blood_depot_.end(), sorting());

	//deletes blood that is out of date
	if (this->GetBloodDepotSize() < N)
		return;

	for (int i = 0; i < N - 1; ++i)
	{
		BloodUnit *to_delete = blood_depot_.back();
		delete to_delete;
		blood_depot_.pop_back();
		counter_utilized_blood++;
	}
}

void Hospital::AddOrderedBlood(int N, int T)
{
	//when ordered blood units arrive, add them to
	//blood_depot_ with specified time of expiration

	for (int i = 0; i < N; ++i)
	{
		BloodUnit* blood_unit = new BloodUnit(T);
		blood_depot_.push_back(blood_unit);

		//EUtilization *utilization = new EUtilization(this);
		//utilization->SetEventTime(blood_unit->GetTimeToExpiration);
		//time_events.push_back(utilization);
	}

	std::sort(blood_depot_.begin(), blood_depot_.end(), sorting());
}

void Hospital::AddDonorsBlood(BloodUnit* blood_unit)
{
	//no need to sort, T2 always > T1
	blood_depot_.push_back(blood_unit);
}

//********************* Constructor *********************
//					  10      25     12       300       30
Hospital::Hospital(int R, int N, int Q) : TU_(300), TB_(30) // R_(300), N_(200), Q_(60),
{
	R_ = R;
	N_ = N;
	Q_ = Q;
	flag_standard_order_ = false;
	flag_emergency_order_ = false;
}