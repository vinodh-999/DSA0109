// Task 1 alternative: ONE Staff class with a type field and a switch inside pay()
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
enum Type { PROF, LAB, GUEST };
struct Staff {
    Type type; std::string name; int id; double basic, research, hours, rate;   // unused fields for most types
    double pay() const {
        switch (type) {
            case PROF:  return basic + basic * 0.42 + research;
            case LAB:   return basic + basic * 0.42;
            case GUEST: return hours * rate;
        }
        return 0;                                // new Type forgotten here -> silent wrong answer
    }
};
// virtual version used only for the size / timing comparison
struct SBase { std::string name; int id; double basic; virtual double pay() const = 0; virtual ~SBase() = default; };
struct SProf : SBase { double research = 0; double pay() const override { return basic + basic * 0.42 + research; } };
struct SGuest : SBase { double hours = 0, rate = 0; double pay() const override { return hours * rate; } };
int main() {
    std::cout << "sizeof(single Staff, every object)   = " << sizeof(Staff) << '\n';
    std::cout << "sizeof(Professor-like derived)       = " << sizeof(SProf) << '\n';
    std::cout << "sizeof(GuestFaculty-like derived)    = " << sizeof(SGuest) << '\n';
    const int N = 20000000;
    std::vector<Staff> a(1000, Staff{PROF, "x", 1, 1000, 5, 0, 0});
    a[1].type = GUEST; a[1].hours = 3; a[1].rate = 2; a[2].type = LAB;
    std::vector<SBase*> b;
    for (int i = 0; i < 1000; ++i) { if (i % 3 == 1) { auto* g = new SGuest; g->basic = 1000; g->hours = 3; g->rate = 2; b.push_back(g); }
                                     else { auto* p = new SProf; p->basic = 1000; p->research = 5; b.push_back(p); } }
    double s1 = 0, s2 = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) s1 += a[i % 1000].pay();
    auto t1 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) s2 += b[i % 1000]->pay();
    auto t2 = std::chrono::steady_clock::now();
    auto ms = [](auto x, auto y) { return std::chrono::duration<double, std::milli>(y - x).count(); };
    std::cout << N << " pay() calls: switch " << ms(t0, t1) << " ms, virtual " << ms(t1, t2) << " ms (" << s1 << " " << s2 << ")\n";
    for (auto* p : b) delete p;
    std::cout << "Places to change for a new type: switch version = enum + pay() switch + every other switch/printing code + fields in the one class;"
                 " hierarchy version = one new class.\n";
}
