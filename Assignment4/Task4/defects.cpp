// Task 4.1/4.2: the ORIGINAL buggy design, with output added to the destructors as evidence
#include <iostream>
#include <vector>
class Account { public: double rate() const { return 3.5; } ~Account() { std::cout << "  ~Account\n"; } };
class SeniorAccount : public Account {
    double* history = new double[100];
public:
    double rate() const { return 4.0; }
    ~SeniorAccount() { std::cout << "  ~SeniorAccount (releases history)\n"; delete[] history; }
};
int main() {
    Account* accounts[2] = { new Account, new SeniorAccount };
    std::cout << "rates:\n";
    for (Account* a : accounts) std::cout << a->rate() << "\n";        // DEFECT 1: prints 3.5 and 3.5
    std::cout << "deleting through Account*:\n";
    for (Account* a : accounts) delete a;                              // DEFECT 2: SeniorAccount dtor never runs
    std::cout << "slicing:\n";
    Account copy = SeniorAccount();                                    // DEFECT 3: only the Account part is copied
    std::cout << "copy.rate() = " << copy.rate() << "  sizeof(Account)=" << sizeof(Account) << " sizeof(SeniorAccount)=" << sizeof(SeniorAccount) << '\n';
    std::cout << "end of main\n";
}
