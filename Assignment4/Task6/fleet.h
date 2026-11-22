// Task 6 - Vehicle rental fleet: existing classes (these are NEVER edited when a new type is added)
#pragma once
#include <iomanip>
#include <iostream>
#include <string>

struct ServiceDate {                                  // member class (has-a): every motor vehicle HAS a service due date
    int y, m, d;
    std::string str() const { return std::to_string(d) + "/" + std::to_string(m) + "/" + std::to_string(y); }
};

class Vehicle {                                       // ABSTRACT base: registration + daily rate + rented flag
protected:
    std::string reg; double dailyRate; bool rented = false;
    double lateFee(int lateDays) const { return lateDays * dailyRate * 1.5; }   // late days cost 150% of the daily rate (shared rule)
public:
    Vehicle(const std::string& r, double rate) : reg(r), dailyRate(rate) {}
    virtual ~Vehicle() = default;                     // virtual destructor: delete through Vehicle* is safe
    // charge for `days` rented, `usage` = km-independent consumption (litres or kWh), `lateDays` after due date
    virtual double charge(int days, double usage, int lateDays) const = 0;      // PURE virtual: each type decides
    virtual std::string kind() const = 0;
    virtual void describe(std::ostream& os) const { os << kind() << " " << reg << " Rs" << dailyRate << "/day" << (rented ? " [RENTED]" : " [available]"); }
    const std::string& getReg() const { return reg; }
    bool isRented() const { return rented; }
    void setRented(bool r) { rented = r; }
};

class Bicycle : public Vehicle {
public:
    Bicycle(const std::string& r, double rate) : Vehicle(r, rate) {}
    double charge(int days, double, int lateDays) const override { return days * dailyRate + lateFee(lateDays); }
    std::string kind() const override { return "Bicycle"; }
};

class MotorVehicle : public Vehicle {                 // adds fuel/charge level + service date (member class)
protected:
    double level; ServiceDate service;
public:
    MotorVehicle(const std::string& r, double rate, double lvl, ServiceDate s) : Vehicle(r, rate), level(lvl), service(s) {}
    void describe(std::ostream& os) const override { Vehicle::describe(os); os << " level=" << level << " service due " << service.str(); }
};

class TwoWheeler : public MotorVehicle {
    static constexpr double PRICE_PER_LITRE = 100;
public:
    using MotorVehicle::MotorVehicle;
    double charge(int days, double litres, int lateDays) const override { return days * dailyRate + litres * PRICE_PER_LITRE + lateFee(lateDays); }
    std::string kind() const override { return "TwoWheeler"; }
};

class Car : public MotorVehicle {
protected:
    int seats;
    static constexpr double PRICE_PER_LITRE = 100;
public:
    Car(const std::string& r, double rate, double lvl, ServiceDate s, int seats) : MotorVehicle(r, rate, lvl, s), seats(seats) {}
    double charge(int days, double litres, int lateDays) const override { return days * dailyRate + litres * PRICE_PER_LITRE + lateFee(lateDays); }
    std::string kind() const override { return "Car"; }
    void describe(std::ostream& os) const override { MotorVehicle::describe(os); os << " seats=" << seats; }
};

class ElectricCar : public Car {                      // is-a Car, but charged per kWh
    double batteryKWh;
    static constexpr double PRICE_PER_KWH = 12;
public:
    ElectricCar(const std::string& r, double rate, double lvl, ServiceDate s, int seats, double kwh) : Car(r, rate, lvl, s, seats), batteryKWh(kwh) {}
    double charge(int days, double kWhUsed, int lateDays) const override { return days * dailyRate + kWhUsed * PRICE_PER_KWH + lateFee(lateDays); }
    std::string kind() const override { return "ElectricCar"; }
    void describe(std::ostream& os) const override { Car::describe(os); os << " battery=" << batteryKWh << "kWh"; }
};

class Fleet {                                         // stores the fleet as an array of base-class pointers
    static constexpr int MAX = 20;
    Vehicle* v[MAX]; int n = 0;
public:
    Fleet() = default;
    Fleet(const Fleet&) = delete; Fleet& operator=(const Fleet&) = delete;
    ~Fleet() { for (int i = 0; i < n; ++i) delete v[i]; }              // one delete per new, through virtual dtor
    bool add(Vehicle* p) { if (n >= MAX) { delete p; return false; } v[n++] = p; return true; }   // fleet takes ownership
    Vehicle* find(const std::string& reg) const { for (int i = 0; i < n; ++i) if (v[i]->getReg() == reg) return v[i]; return nullptr; }
    bool rent(const std::string& reg) { Vehicle* p = find(reg); if (!p || p->isRented()) return false; p->setRented(true); return true; }
    // returns the charge, or -1 when the vehicle is unknown / not rented
    double giveBack(const std::string& reg, int days, double usage, int lateDays) {
        Vehicle* p = find(reg); if (!p || !p->isRented()) return -1;
        p->setRented(false); return p->charge(days, usage, lateDays);      // polymorphic call
    }
    void list() const { for (int i = 0; i < n; ++i) { v[i]->describe(std::cout); std::cout << '\n'; } }
};
