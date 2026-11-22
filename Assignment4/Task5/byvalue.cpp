// Task 5: what happens if add() returns Cart instead of Cart&
#include <iostream>
struct CartV { double sum = 0; int copies = 0;
    CartV() = default; CartV(const CartV& o) : sum(o.sum), copies(o.copies + 1) {}
    CartV add(double p) { sum += p; return *this; }          // returns a COPY
};
struct CartR { double sum = 0;
    CartR& add(double p) { sum += p; return *this; }         // returns the object itself
};
int main() {
    CartV v; v.add(60).add(120).add(180);
    CartR r; r.add(60).add(120).add(180);
    std::cout << "returning Cart  : original cart total = " << v.sum << " (only the first add reached it; others changed temporary copies)\n";
    std::cout << "returning Cart& : original cart total = " << r.sum << '\n';
}
