// Task 3.1-3.4: constructors with initializer lists, member classes, construction/destruction trace
#include <iostream>
#include <string>
#ifdef NOVIRTUAL
#define V
#else
#define V virtual
#endif

class Address {                                  // member class of Person
    std::string city; int pin;
public:
    Address(const std::string& c, int p) : city(c), pin(p) { std::cout << "  Address(" << c << ") "; }
    ~Address() { std::cout << "  ~Address "; }
    std::string str() const { return city + "-" + std::to_string(pin); }
};
class Course {                                   // member class of Student
    std::string title;
public:
    explicit Course(const std::string& t) : title(t) { std::cout << "  Course(" << t << ") "; }
    ~Course() { std::cout << "  ~Course "; }
    const std::string& getTitle() const { return title; }
};

class Person {
protected:
    std::string name; int id; Address addr;
public:
    Person(const std::string& n, int i, const std::string& city, int pin) : name(n), id(i), addr(city, pin) { std::cout << "Person(" << n << ")\n"; }
    virtual ~Person() { std::cout << "~Person\n"; }
};
class Student : public V Person {
protected:
    double cgpa; Course course;
public:
    Student(const std::string& n, int i, const std::string& city, int pin, double c, const std::string& crs)
        : Person(n, i, city, pin), cgpa(c), course(crs) { std::cout << "Student\n"; }
    ~Student() { std::cout << "~Student\n"; }
};
class Staff : public V Person {
protected:
    double salary;
public:
    Staff(const std::string& n, int i, const std::string& city, int pin, double s)
        : Person(n, i, city, pin), salary(s) { std::cout << "Staff\n"; }
    ~Staff() { std::cout << "~Staff\n"; }
};
class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant(const std::string& n, int i, const std::string& city, int pin, double c, const std::string& crs, double s)
#ifdef NOVIRTUAL
        : Student(n, i, city, pin, c, crs), Staff(n, i, city, pin, s) { std::cout << "TeachingAssistant\n"; }
    // without virtual: calling Person(...) here would be an error (Person is an ambiguous base)
#else
        : Person(n, i, city, pin), Student(n, i, city, pin, c, crs), Staff(n, i, city, pin, s) { std::cout << "TeachingAssistant\n"; }
    // with virtual: the MOST-DERIVED class builds the one shared Person; Student/Staff's Person(...) calls are skipped
#endif
    ~TeachingAssistant() { std::cout << "~TeachingAssistant\n"; }
};

int main() {
    std::cout << "---- Student ----\n";        { Student s("Kavya", 1, "Salem", 636001, 8.7, "OOP"); }
    std::cout << "---- Staff ----\n";          { Staff f("Ravi", 2, "Chennai", 600001, 30000); }
    std::cout << "---- TeachingAssistant ----\n"; { TeachingAssistant t("Meena", 3, "Erode", 638001, 9.0, "DSA", 15000); }
    std::cout << "sizeof(TeachingAssistant) = " << sizeof(TeachingAssistant) << "  sizeof(Student) = " << sizeof(Student) << '\n';
}
