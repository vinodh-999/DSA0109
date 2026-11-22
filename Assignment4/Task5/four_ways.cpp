// Task 5.1 + 5.2: four ways to reach a Cart, chaining with *this, and the highest-total search
#include <iomanip>
#include <iostream>
#include <string>

class Cart {
    std::string owner; double sum = 0; double discount = 0;
public:
    explicit Cart(const std::string& owner = "?") { this->owner = owner; }   // this-> separates member from parameter
    Cart& add(const std::string&, double price) { sum += price; return *this; }
    Cart& setDiscount(double discount) { this->discount = discount; return *this; }
    double total() const { return sum * (100 - discount) / 100; }
    const std::string& who() const { return owner; }
    const Cart* address() const { return this; }                              // this is a pointer to the current object
};
void viaReference(Cart& r) { r.add("Milk", 50); }                            // (c) reference: alias, never null, cannot be reseated

int main() {
    std::cout << std::fixed << std::setprecision(2);
    Cart a("A");  a.add("Rice", 60).add("Dal", 120).setDiscount(10);          // (a) directly by name
    Cart b("B");  Cart* p = &b; p->add("Oil", 180); (*p).setDiscount(5);      // (b) pointer with ->
    Cart c("C");  viaReference(c);                                            // (c) reference
    Cart* d = new Cart("D"); d->add("Sugar", 90).add("Tea", 110);             // (d) new / delete
    std::cout << "a=" << a.total() << " b=" << b.total() << " c=" << c.total() << " d=" << d->total() << '\n';
    std::cout << "this == &a ? " << std::boolalpha << (a.address() == &a) << '\n';

    // Task 5.2: array of pointers to carts, find highest total
    Cart* active[4] = { &a, &b, &c, d };
    Cart* best = active[0];
    for (int i = 1; i < 4; ++i) if (active[i]->total() > best->total()) best = active[i];
    std::cout << "highest total: cart " << best->who() << " = " << best->total() << "  (indexing)\n";
    Cart** q = active; Cart* best2 = *q;
    for (Cart** it = active + 1; it != active + 4; ++it) if ((*it)->total() > best2->total()) best2 = *it;
    std::cout << "highest total: cart " << best2->who() << " = " << best2->total() << "  (pointer arithmetic)\n";
    delete d;                                                                 // exactly one delete for one new
}
