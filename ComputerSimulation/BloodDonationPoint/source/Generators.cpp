#include <time.h>
#include "Generators.h"
#include <cmath>

double Generator::UniformDistribution()
{
	// tried code below, but results were poor
	//
	//double xi = fmod(630360016 * prev_x_, m_);
	//prev_x_ = xi;
	//if (prev_x_ < 0)
	//	prev_x_ += (m_);
	//return prev_x_ / (m_);

	int x = prev_x_ / c_;
	prev_x_ = b_ * (prev_x_ - c_ * x) - r_ * x;
	if (prev_x_ < 0)
		prev_x_ += (m_);
	return static_cast<double>(prev_x_) / m_;
}

double Generator::UniformDistribution(const int min, const int max)
{
	return UniformDistribution()*(max - min) + min;
}

double Generator::ExponentialDistribution(const double lambda)
{
	//return (-pow(1 / lambda, -1) * log(UniformDistribution()));
	return (round(-pow(1 / lambda, -1) * log(UniformDistribution())));
}

double Generator::NormalDistribution(const double mean, const double dev)
{ 
	double first_draw;
	double second_draw;
	double x;
	
	do
	{
		first_draw = UniformDistribution();
		second_draw = UniformDistribution();
		x = -log(first_draw);
	} while (!(second_draw <= exp(-pow(x - 1, 2) / 2)));
	
	if (UniformDistribution() < 0.5)
		x = x * -1.0;
	
	return x * dev + mean;

	//double x = 0.0;
	//for (int i = 0; i < 12; i++)
	//	x += UniformDistribution();
	//
	//return mean + sqrt(dev)*(x - 6.0);
}

int Generator::GeometricDistribution(const double p)
{
	double i = 0;
	while (UniformDistribution() > 1 / p)
		i++;
	return i + 1;
}

Generator::Generator() : prev_x_(0.0) //or set prev_x_ as time seed
{
	b_ = pow(7, 5); //16807
	m_ = pow(2, 31) - 1;

	//init random seed to avoid
	//the same randoms in simulation
	srand(time(NULL));
	prev_x_ = rand() * 1000 % 101;
}

Generator::Generator(int seed)
{
	c_ = 127773;
	b_ = pow(7, 5); //16807
	m_ = pow(2, 31) - 1;
	r_ = 2836;
	prev_x_ = seed;
}
