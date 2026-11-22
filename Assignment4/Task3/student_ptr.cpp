// Task 3 alternative: Student holds Address* (heap) -> Rule of Three required
#include <chrono>
#include <iostream>
#include <string>
class Address {
    std::string city; int pin;
public:
    Address(const std::string& c, int p) : city(c), pin(p) {}
    std::string str() const { return city + "-" + std::to_string(pin); }
};
class Student {
    long long regNo;
    Address* home;                                              // 8 extra bytes + heap block
public:
    Student(long long r, const std::string& c, int p) : regNo(r), home(new Address(c, p)) {}     // allocate
    Student(const Student& o) : regNo(o.regNo), home(new Address(*o.home)) {}                   // deep copy
    Student& operator=(const Student& o) { if (this != &o) { Address* n = new Address(*o.home); delete home; home = n; regNo = o.regNo; } return *this; }
    ~Student() { delete home; }                                                                  // release
    std::string str() const { return std::to_string(regNo) + " " + home->str(); }
};
class StudentM { long long regNo; Address home; public: StudentM(long long r, const std::string& c, int p) : regNo(r), home(c, p) {} };
int main() {
    Student a(192311001, "Salem", 636001), b = a; Student c(1, "x", 1); c = a;
    std::cout << a.str() << " | " << b.str() << " | " << c.str() << '\n';
    std::cout << "sizeof(Student with Address member) = " << sizeof(StudentM) << ", sizeof(Student with Address*) = " << sizeof(Student)
              << " (+ separate heap block of at least " << sizeof(Address) << " bytes)\n";
    const int N = 2000000;
    auto t0 = std::chrono::steady_clock::now(); { long long s = 0; for (int i = 0; i < N; ++i) { StudentM m(i, "Salem", 1); s += (long long)sizeof(m); } if (s < 0) std::cout << s; }
    auto t1 = std::chrono::steady_clock::now(); { long long s = 0; for (int i = 0; i < N; ++i) { Student m(i, "Salem", 1); s += (long long)sizeof(m); } if (s < 0) std::cout << s; }
    auto t2 = std::chrono::steady_clock::now();
    auto ms = [](auto x, auto y) { return std::chrono::duration<double, std::milli>(y - x).count(); };
    std::cout << N << " create+destroy: member " << ms(t0, t1) << " ms, pointer (new/delete) " << ms(t1, t2) << " ms\n";
}
