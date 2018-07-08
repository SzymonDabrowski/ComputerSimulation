#pragma once

extern int counter_blood_units;

class BloodUnit
{
public:
	const int& GetTimeToExpiration() const { return time_to_expiration_; };
	void SetTimeToExpiration(const int time) { time_to_expiration_ = time; };

	BloodUnit(int T);
	~BloodUnit() = default;

private:
	int time_to_expiration_; //T1 (from order) or T2 (from donor)
};
 