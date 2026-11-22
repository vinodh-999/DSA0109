// Task 4 alternative: std::vector<std::unique_ptr<Account>> - no manual delete
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
struct Account { virtual double rate() const { return 3.5; } virtual ~Account() = default; };
struct SeniorAccount : Account { std::unique_ptr<double[]> history{new double[100]}; double rate() const override { return 4.0; } };
int main() {
    std::vector<std::unique_ptr<Account>> v;
    v.push_back(std::make_unique<Account>());
    v.push_back(std::make_unique<SeniorAccount>());
    for (auto& a : v) std::cout << a->rate() << '\n';
    // std::vector<Account> bad; bad.push_back(SeniorAccount());   // still slices: unique_ptr does not prevent it, using Account by value does
    // auto c = v[0];   // ERROR: unique_ptr is not copyable -> accidental copies are rejected at compile time
    for (int n : {1000, 100000, 1000000}) {
        std::vector<Account*> raw; std::vector<std::unique_ptr<Account>> sm;
        for (int i = 0; i < n; ++i) { raw.push_back(new SeniorAccount); sm.push_back(std::make_unique<SeniorAccount>()); }
        double a = 0, b = 0;
        auto t0 = std::chrono::steady_clock::now(); for (auto* p : raw) a += p->rate();
        auto t1 = std::chrono::steady_clock::now(); for (auto& p : sm) b += p->rate();
        auto t2 = std::chrono::steady_clock::now();
        for (auto* p : raw) delete p;
        auto ms = [](auto x, auto y) { return std::chrono::duration<double, std::milli>(y - x).count(); };
        std::cout << "n=" << n << ": raw ptr loop " << ms(t0, t1) << " ms, unique_ptr loop " << ms(t1, t2) << " ms (sums " << a << ", " << b << ")\n";
    }
}
