// Task 2 alternative: TeachingAssistant HAS-A Student and HAS-A Staff (composition)
#include <chrono>
#include <iostream>
#include <string>
struct Person { std::string name; int id; std::string phone; };
struct Student : virtual Person { std::string programme; double cgpa; };
struct Staff   : virtual Person { std::string department; double salary; };
struct TAvirtual : Student, Staff {};                     // inheritance: ONE Person
struct StudentC { Person p; std::string programme; double cgpa; };
struct StaffC   { Person p; std::string department; double salary; };
struct TAcomp { StudentC s; StaffC f; };                  // composition: TWO Person copies
struct StudentN : Person { std::string programme; double cgpa; };   // non-virtual, for comparison
struct StaffN   : Person { std::string department; double salary; };
struct TAnon : StudentN, StaffN {};
int main() {
    std::cout << "sizeof(Person)                         = " << sizeof(Person) << '\n';
    std::cout << "sizeof(TA, virtual inheritance)        = " << sizeof(TAvirtual) << "  (1 Person + vbptrs)\n";
    std::cout << "sizeof(TA, composition)                = " << sizeof(TAcomp) << "  (2 Person)\n";
    std::cout << "sizeof(TA, non-virtual inheritance)    = " << sizeof(TAnon) << "  (2 Person)\n";
    TAcomp c; c.s.p.name = "from student side"; c.f.p.name = "from staff side";
    std::cout << "composition: two names exist -> '" << c.s.p.name << "' and '" << c.f.p.name << "' (can disagree)\n";
    TAvirtual v; v.name = "one shared name"; std::cout << "virtual inheritance: v.name = '" << v.name << "' (one unambiguous name)\n";
    // time to reach the name field
    const int N = 100000000; volatile size_t sink = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) sink = sink + v.name.size();
    auto t1 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) sink = sink + c.s.p.name.size();
    auto t2 = std::chrono::steady_clock::now();
    auto ms = [](auto a, auto b) { return std::chrono::duration<double, std::milli>(b - a).count(); };
    std::cout << N << " accesses: via virtual base " << ms(t0, t1) << " ms, via member " << ms(t1, t2) << " ms\n";
}
