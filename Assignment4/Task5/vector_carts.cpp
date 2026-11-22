// Task 5 alternative: std::vector<Cart> objects instead of an array of pointers
#include <chrono>
#include <iostream>
#include <vector>
struct Cart { double items[10] = {}; int n = 0; double discount = 0; double total() const { double t = 0; for (int i = 0; i < n; ++i) t += items[i]; return t * (100 - discount) / 100; } };
int main() {
    std::vector<Cart> v; v.reserve(1);
    v.push_back(Cart());
    Cart* p = &v[0];                       // pointer INTO the vector
    const void* before = v.data();
    for (int i = 0; i < 10; ++i) v.push_back(Cart());   // growth reallocates and copies/moves elements
    std::cout << "vector data pointer changed after growth: " << std::boolalpha << (before != (const void*)v.data())
              << "; old pointer p " << (p == &v[0] ? "still" : "no longer") << " points at element 0 (dangling)\n";
    std::cout << "sizeof(Cart)=" << sizeof(Cart) << " bytes copied per element on growth; pointer array stores " << sizeof(Cart*) << " bytes per cart (+ the Cart on the heap)\n";
    const int N = 1000000;
    std::vector<Cart> objs(N); std::vector<Cart*> ptrs; std::vector<Cart*> own;
    for (int i = 0; i < N; ++i) { objs[i].n = 1; objs[i].items[0] = (i * 7) % 1000; own.push_back(new Cart(objs[i])); ptrs.push_back(own.back()); }
    double best = -1; auto t0 = std::chrono::steady_clock::now();
    for (auto& c : objs) if (c.total() > best) best = c.total();
    auto t1 = std::chrono::steady_clock::now();
    for (auto* c : ptrs) if (c->total() > best) best = c->total();
    auto t2 = std::chrono::steady_clock::now();
    auto ms = [](auto a, auto b) { return std::chrono::duration<double, std::milli>(b - a).count(); };
    std::cout << N << " carts, find max: vector<Cart> " << ms(t0, t1) << " ms, array of pointers " << ms(t1, t2) << " ms (best " << best << ")\n";
    for (auto* c : own) delete c;
}
