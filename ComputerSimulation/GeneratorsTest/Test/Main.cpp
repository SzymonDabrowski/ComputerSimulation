#include "/repos/ComputerSimulation/BloodDonationPoint/source/Generators.h"
#include <iostream>
#include <fstream>
#include <string>

//use *data.csv only
//count density and distribution
//using matlab scripts

int main()
{
	std::string name_data = "data_";
	std::string name_distribuant = "distribuant_";
	std::string name_density = "density_";
	std::string format = ".csv";

	const int size = 1000;
	double density[size] = { 0 };
	double distribuant[size] = { 0 };

	int seed = 0;

	for (int i = 0; i < 10; ++i)
	{
		seed += 100000;
		Generator *generator = new Generator(seed);
		double sum = 0;
		double random_array[100000] = { 0 };

		std::ofstream output;
		output.open(name_data + std::to_string(i) + format);
		for (int j = 0; j < 100000; ++j)
		{
			random_array[j] = generator->UniformDistribution(5,10);
			sum += random_array[j];
			output << random_array[j];
			output << "\n";
		}
		double mean = sum / 100000;
		output << "\n";
		output << mean;
		output.close();
		
		std::ofstream output_disitribuant;
		std::ofstream output_density;
		output_disitribuant.open(name_distribuant + std::to_string(i) + format);
		output_density.open(name_density + std::to_string(i) + format);
		double prob = 0;// EXP, UNIFORM, GEOM
		double distribuant_array[size] = { 0 };
		double density_array[size] = { 0 };
		//double SUM = 0;
		double max = 0;
		//double min = 0;
		for (int i = 0; i < 100000; ++i)
		{
			if (random_array[i] > max)
				max = random_array[i];
			//if (random_array[i] < min)
			//	min = random_array[i];
		}
		double prob_step = max / size;
		//double prob_step = max / size; //EXPONENTIAL
		//double prob_step = 1.0 / size; //UNIFORM 0-1
		for (int j = 0; j < size; ++j)// UNIFORM
		{
			//prob += 1; //discrete exp
			prob += prob_step;// UNIFORM 0.001 (1/size), non-discrete not 0-1 max/size... AND EXPONENTIAL
			
			for (int i = 0; i < 100000; ++i)
			{
				if (random_array[i] <= prob)
				{
					++distribuant_array[j];
					++distribuant[j];
					if (random_array[i] > prob - prob_step) //prob_step for UNIFORM and non-distrete
					{
						++density_array[j];
						++density[j];
					}
				}
			}
			distribuant_array[j] /= 100000;
			density_array[j] /= 100000;
			//SUM += density_array[j];
			output_disitribuant << distribuant_array[j];
			output_disitribuant << "\n";
			output_density << density_array[j];
			output_density << "\n";
			density[j] /= 100000;
			distribuant[j] /= 100000;
		}
		//output_density << "\n";
		//output_density << SUM;
		output_disitribuant.close();
		output_density.close();

		delete generator;
	}

	//for (int i = 0; i < size; ++i)
	//{
	//	density[i] /= 10;
	//	distribuant[i] /= 10;
	//}

	return 0;
}