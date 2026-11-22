// Task 2.3 - class declarations for the university (access specifiers + inheritance kinds)
#include <iostream>
#include <string>
class Person {                       // name, ID, phone are common to everyone
protected:
    std::string name; int id; std::string phone;
public:
    Person(const std::string& n, int i, const std::string& p) : name(n), id(i), phone(p) {}
    virtual ~Person() = default;
    std::string getName() const { return name; }
};
class Student : public virtual Person {          // is-a Person: PUBLIC (a student can be used wherever a Person is expected)
protected:
    std::string programme; double cgpa;
public:
    Student(const std::string& n, int i, const std::string& p, const std::string& prog, double c)
        : Person(n, i, p), programme(prog), cgpa(c) {}
};
class Staff : public virtual Person {            // is-a Person: PUBLIC
protected:
    std::string department; double salary;
public:
    Staff(const std::string& n, int i, const std::string& p, const std::string& d, double s)
        : Person(n, i, p), department(d), salary(s) {}
};
class TeachingAssistant : public Student, public Staff {   // multiple inheritance (diamond with virtual Person) = hybrid
public:
    TeachingAssistant(const std::string& n, int i, const std::string& p, const std::string& prog, double c, const std::string& d, double s)
        : Person(n, i, p), Student(n, i, p, prog, c), Staff(n, i, p, d, s) {}
    void show() const { std::cout << name << " (" << id << ") " << programme << " CGPA " << cgpa << ", " << department << " Rs " << salary << '\n'; }
};
class Fellowship {                               // has-a: a fellowship is something a scholar HOLDS, not a kind of student
    double amount;
public:
    explicit Fellowship(double a) : amount(a) {}
    double getAmount() const { return amount; }
};
class ResearchScholar : public Student {         // is-a Student (single inheritance) + has-a Fellowship (member class)
    Fellowship fellowship;
public:
    ResearchScholar(const std::string& n, int i, const std::string& p, const std::string& prog, double c, double f)
        : Person(n, i, p), Student(n, i, p, prog, c), fellowship(f) {}
    void show() const { std::cout << name << " scholar, fellowship Rs " << fellowship.getAmount() << '\n'; }
};
int main() {
    TeachingAssistant ta("Kavya", 1, "99999", "MCA", 8.7, "CSE", 15000);
    ta.show();
    std::cout << "TA name via single shared Person: " << ta.getName() << '\n';
    ResearchScholar rs("Arun", 2, "88888", "PhD", 9.1, 31000);
    rs.show();
}
