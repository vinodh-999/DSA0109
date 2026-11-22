// proves the dangling pointer after vector growth under AddressSanitizer
#include <iostream>
#include <vector>
int main() { std::vector<int> v(1, 7); int* p = &v[0]; v.resize(1000); std::cout << *p << '\n'; }
