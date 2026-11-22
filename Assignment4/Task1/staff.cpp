// Task 1.2 - repaired payroll design: common base class, protected data, pay() overridden
#include <iostream>
#include <memory>
#include <string>
#include <vector>

constexpr double DA_RATE = 0.42;            // named constant replaces the magic number 0.42

class Staff {                               // abstract base: written once
protected:
    std::string name; int id; double basic;
public:
    Staff(const std::string& n, int i, double b) : name(n), id(i), basic(b) {}
    virtual ~Staff() = default;
    virtual double pay() const = 0;         // every category must define its own pay rule
    virtual std::string kind() const = 0;
    const std::string& getName() const { return name; }
    int getId() const { return id; }
protected:
    double basicWithDA() const { return basic + basic * DA_RATE; }   // shared rule, written once
};

class Professor : public Staff {
    double research;
public:
    Professor(const std::string& n, int i, double b, double r) : Staff(n, i, b), research(r) {}
    double pay() const override { return basicWithDA() + research; }
    std::string kind() const override { return "Professor"; }
};
class LabAssistant : public Staff {
public:
    LabAssistant(const std::string& n, int i, double b) : Staff(n, i, b) {}
    double pay() const override { return basicWithDA(); }
    std::string kind() const override { return "LabAssistant"; }
};
class GuestFaculty : public Staff {         // no basic pay: base basic = 0
    double hours, rate;
public:
    GuestFaculty(const std::string& n, int i, double h, double r) : Staff(n, i, 0), hours(h), rate(r) {}
    double pay() const override { return hours * rate; }
    std::string kind() const override { return "GuestFaculty"; }
};

// --- adding a new category later: ONE new class, nothing above changes ---
class Technician : public Staff {
    double overtime;
public:
    Technician(const std::string& n, int i, double b, double o) : Staff(n, i, b), overtime(o) {}
    double pay() const override { return basicWithDA() + overtime; }
    std::string kind() const override { return "Technician"; }
};

int main() {
    std::vector<std::unique_ptr<Staff>> all;
    all.emplace_back(new Professor("Dr. Rao", 101, 50000, 8000));
    all.emplace_back(new LabAssistant("Meena", 102, 20000));
    all.emplace_back(new GuestFaculty("Mr. Das", 201, 12, 1500));
    all.emplace_back(new Technician("Ravi", 301, 25000, 3000));   // new category in the same loop
    double total = 0;
    for (const auto& s : all) {              // ONE loop for every kind of staff
        std::cout << s->kind() << " " << s->getName() << " (" << s->getId() << "): " << s->pay() << '\n';
        total += s->pay();
    }
    std::cout << "Total payroll: " << total << '\n';
    // hand check: Professor 50000+21000+8000=79000; Lab 20000+8400=28400; Guest 18000; Tech 25000+10500+3000=38500; sum 163900
}
