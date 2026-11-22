// Task 4.3: repaired program - virtual functions, virtual destructor, pointers instead of copies
#include <iostream>
#include <memory>
#include <vector>
class Account {
public:
    virtual double rate() const { return 3.5; }
    virtual ~Account() { std::cout << "  ~Account\n"; }          // virtual destructor
};
class SeniorAccount : public Account {
    double* history = new double[100];
public:
    double rate() const override { return 4.0; }                 // override: compiler checks the signature
    ~SeniorAccount() override { std::cout << "  ~SeniorAccount (releases history)\n"; delete[] history; }
    SeniorAccount() = default;
    SeniorAccount(const SeniorAccount&) = delete;                // owning raw pointer: no accidental copies
    SeniorAccount& operator=(const SeniorAccount&) = delete;
};
int main() {
    Account* accounts[2] = { new Account, new SeniorAccount };
    for (Account* a : accounts) std::cout << a->rate() << "\n";   // 3.5 and 4
    std::cout << "deleting through Account*:\n";
    for (Account* a : accounts) delete a;                          // now runs the full destructor chain
    SeniorAccount senior;                                          // store/pass by pointer or reference, never by value
    const Account& ref = senior;
    std::cout << "via reference: " << ref.rate() << '\n';
    // Account copy = senior;  // would still compile (slicing)! -> make Account non-copyable or use pointers; see rules in README
}
