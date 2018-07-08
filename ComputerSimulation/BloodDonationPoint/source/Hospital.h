#pragma once
 
#include <queue>
#include <vector>

#include "Patient.h"
#include "BloodUnit.h"
#include "TimeEvents.h"
#include "Enums.h"

extern int counter_utilized_blood;

class Hospital
{
public:
	void OrderInfo();
	bool FlagInfo(int type);

	void Time(int delta_time);//used in BloodUnit!

	//getters and setter
	const int& GetStandardOrderSize() const; //used in C.Events
	const int& GetEmergencyOrderSize() const; //used in C.Events, consider using enums
	void SetFlag(int type); //sets flag while orderd in delivery
	void ClearFlag(int type); //clears flag when blood is delivered
	int GetBloodDepotSize() const; //returns number of blood units
	int GetQueueSize() const;//returns number of patients in queue
	int NeededBloodUnits(); //checks how much blood needs a patient
	int GetTU();
	int GetTB();

	void DeletePatient();
	void AddPatientToQueue(Patient* patient);
	void UseBlood(int N);
	void DeleteBlood();
	void DeleteBlood(int N); //A4 task
	void AddOrderedBlood(int N, int T);
	void AddDonorsBlood(BloodUnit* blood_unit);

	//flags
	bool IsEnoughBlood() const;
	bool IsQueueEmpty() const;
	
	Hospital(int R, int N, int Q);
	~Hospital() = default;

private:
	bool flag_standard_order_; //if ordered, do not order more until delivery [//set in C.Event]
	bool flag_emergency_order_; //if ordered, do not order more until delivery [//set in C.Event]

	// R, N, Q const if hard coded in constructor
	int R_; //minimum acceptable level of blood units
	int N_; //standard order size
	int Q_; //emergency order size
	const int TU_; //time to A4 tast
	const int TB_; //minimum blood lovel in A4 task
	std::queue<Patient*> patients_queue_;
	std::vector<BloodUnit*> blood_depot_;

	//std::vector<TimeEvent*> time_events;
};
