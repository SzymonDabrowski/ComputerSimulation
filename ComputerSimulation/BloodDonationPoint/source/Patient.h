#pragma once
 
class Patient
{
public:
	const int& GetBloodUnitsNeeded() const { return blood_units_needed_; };

	Patient();
	~Patient() = default;

private:
	const double W_;
	int blood_units_needed_{};
};
