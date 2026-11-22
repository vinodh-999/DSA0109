#include "bus.h"          // fleet.h comes in through bus.h; only main() knows about the new type
#include <cstdlib>
static int failures = 0;
static void check(const char* what, double got, double want) {
    bool ok = std::abs(got - want) < 1e-9; if (!ok) ++failures;
    std::cout << (ok ? "PASS " : "FAIL ") << what << ": got " << got << ", expected " << want << '\n';
}
int main() {
    std::cout << std::fixed << std::setprecision(2);
    Fleet f;
    f.add(new Bicycle("BI-01", 100));
    f.add(new TwoWheeler("TW-01", 300, 8, {2027, 1, 15}));
    f.add(new Car("CA-01", 1500, 30, {2027, 3, 1}, 5));
    f.add(new ElectricCar("EV-01", 2000, 80, {2027, 6, 30}, 5, 40));
    std::cout << "--- fleet ---\n"; f.list();

    std::cout << "--- rent each type and return on time ---\n";
    f.rent("BI-01"); f.rent("TW-01"); f.rent("CA-01"); f.rent("EV-01");
    check("Bicycle 3 days", f.giveBack("BI-01", 3, 0, 0), 300);                 // 3*100
    check("TwoWheeler 2 days 4 L", f.giveBack("TW-01", 2, 4, 0), 1000);         // 600 + 400
    check("Car 2 days 10 L", f.giveBack("CA-01", 2, 10, 0), 4000);              // 3000 + 1000
    check("ElectricCar 2 days 25 kWh", f.giveBack("EV-01", 2, 25, 0), 4300);    // 4000 + 300

    std::cout << "--- late return (2 days late, rate*1.5 per late day) ---\n";
    f.rent("CA-01");
    check("Car 2 days 10 L + 2 late", f.giveBack("CA-01", 2, 10, 2), 4000 + 2 * 1500 * 1.5);   // 8500
    f.rent("BI-01");
    check("Bicycle 1 day + 1 late", f.giveBack("BI-01", 1, 0, 1), 100 + 150);

    std::cout << "--- error cases ---\n";
    std::cout << "rent unknown: " << std::boolalpha << f.rent("XX-99") << ", return not-rented: " << f.giveBack("TW-01", 1, 0, 0) << '\n';
    f.rent("TW-01"); std::cout << "rent twice: " << f.rent("TW-01") << '\n';

    std::cout << "--- NEW TYPE added without changing existing classes ---\n";
    f.add(new ElectricBus("EB-01", 6000, 100, {2027, 9, 9}, 40));
    f.rent("EB-01");
    check("ElectricBus 1 day 50 kWh", f.giveBack("EB-01", 1, 50, 0), 6000 + 50 * 9.5);        // 6475
    f.list();
    std::cout << (failures == 0 ? "ALL TESTS PASSED\n" : "SOME TESTS FAILED\n");
    return failures;
}
