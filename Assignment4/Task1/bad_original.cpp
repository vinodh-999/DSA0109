// Task 1.1: the original three classes cannot be processed in one loop
#include <iostream>
#include <string>
using namespace std;
class Professor   { public: string name; int id; double basic; double da; double research;
                    double pay() { return basic + basic * 0.42 + research; } };
class LabAssistant{ public: string name; int id; double basic; double da;
                    double pay() { return basic + basic * 0.42; } };
class GuestFaculty{ public: string nm; int code; double hours; double rate;
                    double pay() { return hours * rate; } };
int main() {
    Professor p{"Rao", 1, 50000, 0, 8000}; GuestFaculty g{"Das", 2, 12, 1500};
    cout << p.pay() << ' ' << g.pay() << '\n';
    cout << "sizeof Professor=" << sizeof(Professor) << " LabAssistant=" << sizeof(LabAssistant) << " GuestFaculty=" << sizeof(GuestFaculty) << '\n';
    // Professor* arr[] = { &p, &g };   // ERROR: no common type -> no single loop
    p.basic = -5;                       // public data: anyone can put invalid data
    cout << "pay with invalid basic: " << p.pay() << " (no protection)\n";
    // g.name  -> ERROR: GuestFaculty uses 'nm'/'code' for the same information
}
