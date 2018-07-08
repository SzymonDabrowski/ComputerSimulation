#pragma once
#include <cmath>

class Generator
{
public:
	double UniformDistribution();
	double UniformDistribution(const int min, const int max);
	double ExponentialDistribution(const double lambda); //lambda^-1 is mean value
	double NormalDistribution(const double mean, const double dev);
	int GeometricDistribution(const double p);

	Generator();
	Generator(int seed);
	~Generator() = default;

private:
	int c_;
	int b_;
	int m_;
	int r_;
	double prev_x_;
};