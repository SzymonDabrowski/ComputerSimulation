//TODO:
//	- set seeds in main and use them in generators!
//	- tryb krokowy i ci¹g³y (switch)
//	- a zale¿ne od czasu [OK]

#include <list>
#include <iostream>
#include <fstream>
#include <string>

#include "Hospital.h"
#include "ConditionalEvents.h"
#include "TimeEvents.h"
#include "Enums.h"
#include "BloodUnit.h" //needed for extern int counter_blood_units

// uncomment:
// PRINTOUT to see couts of iter (in future all couts)
// PLOT_DATA to save data to csv file (to plots)
// PROBABILITY to save data about probability
//#define PRINTOUT 1
//#define PLOT_DATA 2
#define PROBABILITY 3

void GetInfo(std::vector<TimeEvent*> time_events, Hospital *hospital); //displays info about systems state
void ExitInfo(double A, double B);

struct sorting
{
	inline bool operator() (const TimeEvent* time_event_1, const TimeEvent* time_event_2)
	{
		return (time_event_1->GetEventTime() > time_event_2->GetEventTime());
	}
};

int main()
{
	int type = 0;
	std::cout << "1: continuous\n2: step-by-step\n" << std::endl;

	std::cin >> type;
	while (type != 1 && type != 2)
	{
		std::cout << "Wrong number. Type 1 for continuous or 2 for step-by-step.\n";
		std::cin >> type;
	}

	double A;
	double B;

	#ifdef PROBABILITY
	double A_array[36] = { 0 };

	std::string name_data = "A";
	std::string format = ".csv";
	std::ofstream output;
	output.open(name_data + format);
	#endif

	int R = 10;
	int N = 25;
	int Q = 60;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			int counter_orders = 0;
			int counter_emergency_orders = 0;
			A = 0;  // A is the probability of emergency order

			//int counter_blood_units = 0; //extern, is in BloodUnit constructor
			//int counter_utilized_blood = 0; //extern, is in Hospital
			B = 0; // B is the percentage of utilized blood

			//int max_queue_size = 100;

			Hospital *hospital = new Hospital(R, N, Q);
			bool flag_system_change = true;
			bool flag_time_events = false;
			bool flag_A4 = false;
			int A4_time = 0;

			EPatientService *patient_service = new EPatientService(hospital);
			EStandardOrder *standard_order = new EStandardOrder(hospital);
			EEmergencyOrder *emergency_order = new EEmergencyOrder(hospital);

			std::vector<TimeEvent*> time_events;
			EPatientArrival *patient_arrival = new EPatientArrival(hospital);
			EDonorArrival *donor_arrival = new EDonorArrival(hospital);
			time_events.push_back(patient_arrival);
			time_events.push_back(donor_arrival);

			std::sort(time_events.begin(), time_events.end(), sorting());

			//open file to save data 
			#ifdef PLOT_DATA
			std::string name_data = "R_";
			name_data = name_data + std::to_string(R) + " N_"  + std::to_string(N); //ex. "R_25 N_80"
			std::string format = ".csv";
			std::ofstream output;
			output.open(name_data + format);
			#endif

			#pragma region simulation-type-COMMENTED
			//int type = 0;
			//std::cout << "1: continuous\n2: step-by-step\n" << std::endl;

			//std::cin >> type;
			//while (type != 1 && type != 2)
			//{
			//	std::cout << "Wrong number. Type 1 for continuous or 2 for step-by-step.\n";
			//	std::cin >> type;
			//}
			#pragma endregion
			int sys_time = 0;
			int end_simulation = 10000; //10 000 orders
			int aux = 0;

			while (counter_orders < end_simulation) //>=counter_orders
			// time is better than orders count because it is not random
			{
				//#if PRINTOUT
				//std::cout << std::endl << "Iteration " << it << std::endl;
				//#endif 
				aux = counter_orders;

				flag_system_change = true;
				while (flag_system_change)
				{
					flag_system_change = false;
					while (!flag_time_events)
					{
						while (!time_events.empty())
						{
							if (time_events.back()->GetEventTime() == 0)
							{
								if (time_events.back()->GetEventName() != utilization)
									time_events.back()->Execute();

								int event_name = time_events.back()->GetEventName();
								switch (event_name)
								{
								case patient_arrived:
								{
									EPatientArrival * next_patient = new EPatientArrival(hospital);
									next_patient->SetEventTime(time_events.back()->GetEventTime());
									TimeEvent *to_delete = time_events.back();
									delete to_delete;
									time_events.pop_back();
									time_events.push_back(next_patient);
									break;
								}
								case donor_arrived:
								{
									EDonorArrival * next_donor = new EDonorArrival(hospital);
									next_donor->SetEventTime(time_events.back()->GetEventTime());
									TimeEvent *to_delete = time_events.back();
									delete to_delete;
									time_events.pop_back();
									time_events.push_back(next_donor);

									EUtilization *utilization = new EUtilization(hospital);
									utilization->SetEventTime(time_events.back()->GetTimeOfExpiration());
									time_events.push_back(utilization);
									break;
								}
								case blood_delivery:
								{
									int time = time_events.back()->GetTimeOfExpiration();
									TimeEvent *to_delete = time_events.back();
									delete to_delete;
									time_events.pop_back();

									EUtilization *utilization = new EUtilization(hospital);
									utilization->SetEventTime(time);
									time_events.push_back(utilization);
									break;
								}
								case A4:
								{
									TimeEvent *to_delete = time_events.back();
									delete to_delete;
									time_events.pop_back();
									break;
								}
								default: //case utilization:
								{		 //remember that EUtilization is executed even when blood units were used and are no longer in system
									time_events.back()->Execute();

									TimeEvent *to_delete = time_events.back();
									delete to_delete;
									time_events.pop_back();
									break;
								}
								}

								std::sort(time_events.begin(), time_events.end(), sorting());
								flag_system_change = true;
							}
							else
							{
								flag_time_events = true;
								break;
							}
						}
						//flag_time_events = true;
						//no need because there always is event in agenda
					}

					if (!hospital->IsEnoughBlood())
					{
						if (!hospital->FlagInfo(standard))
						{
							standard_order->Execute();

							//drawing time in EDelivery class would provide less arguments in constructor
							int event_time = standard_order->GetEventTime();
							EDelivery *delivery = new EDelivery(hospital, event_time, standard);
							time_events.push_back(delivery);

							std::sort(time_events.begin(), time_events.end(), sorting());

							if (sys_time > 3400000)
							{
							counter_orders++;
							}

							flag_system_change = true;
						}
					}

					if (!hospital->IsQueueEmpty())
					{
						if ((hospital->NeededBloodUnits() < hospital->GetBloodDepotSize()))
						{
							patient_service->Execute();
							flag_system_change = true;
						}
						else
						{
							if (!hospital->FlagInfo(emergency))
							{
								emergency_order->Execute();

								//drawing time in EDelivery class would provide less arguments in constructor
								int event_time = emergency_order->GetEventTime();
								EDelivery *delivery = new EDelivery(hospital, event_time, emergency);
								time_events.push_back(delivery);

								std::sort(time_events.begin(), time_events.end(), sorting());

								if (sys_time > 3400000)
								{
								counter_orders++;
								counter_emergency_orders++;
								}

								flag_system_change = true;
							}
						}
					}

					if (!flag_A4)
					{
						if (hospital->GetBloodDepotSize() < hospital->GetTB())
						{
							A4_time = 0;
						}
						else
						{
							if (A4_time == hospital->GetTU())
							{
								EA4Utilization *A4Utilization = new EA4Utilization(hospital);
								time_events.push_back(A4Utilization);

								std::sort(time_events.begin(), time_events.end(), sorting());

								flag_system_change = true;
								flag_time_events = false;
							}
							flag_A4 = true;
						}
					}
				}
				flag_time_events = false;

				//data export to outer file
				//export double to string and change . to , 
				#ifdef PLOT_DATA
				if (aux != counter_orders)
				{
					output << A << " " << sys_time; //counter_orders;
					//output << hospital->GetBloodDepotSize() << "," << sys_time;
					//output << hospital->GetQueueSize() << "," << sys_time;
					output << "\n";
				}
				#endif

				//time actualization
				int delta_time = time_events.back()->GetEventTime(); //Tmin
				//sys_time += delta_time;
				if (flag_A4)
				{
					if (delta_time + A4_time < hospital->GetTU())
					{
						A4_time += delta_time;
						hospital->Time(delta_time);  //change blood time and patients in queue
						for (int i = 0; i < time_events.size(); ++i)
						{
							time_events[i]->UpdateTime(delta_time);
						}
						sys_time += delta_time;
					}
					else
					{
						delta_time += A4_time;
						A4_time = 300;
						hospital->Time(delta_time); //change blood time
						for (int i = 0; i < time_events.size(); ++i)
						{
							time_events[i]->UpdateTime(delta_time);
						}
						sys_time += delta_time;
					}
					flag_A4 = false;
				}
				else
				{
					hospital->Time(delta_time); //change blood time
					for (int i = 0; i < time_events.size(); ++i)
					{
						time_events[i]->UpdateTime(delta_time);
					}
					sys_time += delta_time;
				}

				// Statistics
				if (counter_orders > 0)
				{
					A = static_cast<double>(counter_emergency_orders) / counter_orders;
				}
				//if (A < 0.07 && sys_time > 200000)
				//	break;

				if (counter_blood_units != 0)
					B = static_cast<double>(counter_utilized_blood) / counter_blood_units;

				//if (hospital->GetQueueSize() > max_queue_size)
				//{
				//	std::cout << "Too many patients in queue" << std::endl;
				//	break;
				//}

				if (type == 2)
				{
					GetInfo(time_events, hospital);
					getchar();
					//if (it == 0)
					//	getchar();
				}
			}

			#ifdef PROBABILITY
			A_array[6 * i + j] = A;
			output << A << " " << B << " "<< N << " " << R << "\n";
			#endif
			B = 0;
			counter_blood_units = 0;
			counter_utilized_blood = 0;

			//close file
			#ifdef PLOT_DATA
			output.close();
			#endif

			delete hospital;
			for (int i = 0; i < time_events.size(); i++)
			{
				TimeEvent* to_delete = time_events.back();
				time_events.pop_back();
				delete to_delete;
			}

			N += 70;
		}
		N = 25;
		R += 70;
	}

	#ifdef PROBABILITY
	output.close();
	#endif

	ExitInfo(A,B);
	getchar();
	getchar();

	return 0;
}

void GetInfo(std::vector<TimeEvent*> time_events, Hospital *hospital)
{
	std::cout << std::endl;
	//std::cout << "=========================" << std::endl;
	std::cout << "          INFO           " << std::endl;
	std::cout << "Number of time events in agenda: " << time_events.size() << std::endl;
	std::cout << "Number of blood units in depot: " << hospital->GetBloodDepotSize() << std::endl;
	std::cout << "Any orders?: ";
	hospital->OrderInfo();
	std::cout << std::endl;
	std::cout << "Queue size: " << hospital->GetQueueSize() << std::endl;
	std::cout << "        ENDINFO          " << std::endl;
	std::cout << "=========================" << std::endl;
}

void ExitInfo(double A, double B)
{
	std::cout << std::endl;
	std::cout << "Simulation executed" << std::endl;
	std::cout << "Probability of emergency order is: " << A << std::endl;
	std::cout << "Percent of utilized blood units: " << B << std::endl;
}