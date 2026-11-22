// Task 6 alternative: ONE Vehicle class, type code and a switch in charge()
#include <chrono>
#include <iostream>
#include <vector>
#include "fleet.h"
enum VType { BICYCLE, TWOWHEELER, CAR, EV };
struct VehicleS {
    VType type; std::string reg; double dailyRate; bool rented = false;
    double level = 0; int serviceY = 0, serviceM = 0, serviceD = 0;   // unused by bicycles
    int seats = 0; double batteryKWh = 0;                              // unused by bicycles / two-wheelers / non-EV
    double charge(int days, double usage, int late) const {
        double late_ = late * dailyRate * 1.5;
        switch (type) {
            case BICYCLE:    return days * dailyRate + late_;
            case TWOWHEELER:
            case CAR:        return days * dailyRate + usage * 100 + late_;
            case EV:         return days * dailyRate + usage * 12 + late_;
        }
        return 0;   // a new type forgotten here silently returns 0
    }
};
int main() {
    std::cout << "sizeof(VehicleS, every object) = " << sizeof(VehicleS) << '\n';
    std::cout << "sizeof(Bicycle) = " << sizeof(Bicycle) << ", TwoWheeler = " << sizeof(TwoWheeler) << ", Car = " << sizeof(Car) << ", ElectricCar = " << sizeof(ElectricCar) << '\n';
    const int N = 40000000; const int K = 1000;
    std::vector<VehicleS> a; std::vector<Vehicle*> b;
    for (int i = 0; i < K; ++i) {
        VType t = (VType)(i % 4); a.push_back({t, "r", 100.0 + i, false}); 
        switch (t) { case BICYCLE: b.push_back(new Bicycle("r", 100.0 + i)); break;
                     case TWOWHEELER: b.push_back(new TwoWheeler("r", 100.0 + i, 1, {2027,1,1})); break;
                     case CAR: b.push_back(new Car("r", 100.0 + i, 1, {2027,1,1}, 5)); break;
                     default: b.push_back(new ElectricCar("r", 100.0 + i, 1, {2027,1,1}, 5, 40)); }
    }
    double s1 = 0, s2 = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) s1 += a[i % K].charge(2, 3, 1);
    auto t1 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) s2 += b[i % K]->charge(2, 3, 1);
    auto t2 = std::chrono::steady_clock::now();
    auto ms = [](auto x, auto y) { return std::chrono::duration<double, std::milli>(y - x).count(); };
    std::cout << N << " charge() calls: switch " << ms(t0, t1) << " ms, virtual " << ms(t1, t2) << " ms, results equal: " << (s1 == s2) << '\n';
    for (auto* p : b) delete p;
    std::cout << "New vehicle type: switch version edits enum + charge() switch + describe/list code + adds unused fields to the single class (>= 3 edits in existing code);"
                 " hierarchy version adds one new class (bus.h) and edits 0 existing classes.\n";
}
