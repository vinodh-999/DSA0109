// EXTENSION: a new vehicle type added in its own file. fleet.h is NOT modified.
#pragma once
#include "fleet.h"
class ElectricBus : public MotorVehicle {
    int seats; double kWhPrice = 9.5;
public:
    ElectricBus(const std::string& r, double rate, double lvl, ServiceDate s, int seats) : MotorVehicle(r, rate, lvl, s), seats(seats) {}
    double charge(int days, double kWhUsed, int lateDays) const override { return days * dailyRate + kWhUsed * kWhPrice + lateFee(lateDays); }
    std::string kind() const override { return "ElectricBus"; }
    void describe(std::ostream& os) const override { MotorVehicle::describe(os); os << " seats=" << seats; }
};
