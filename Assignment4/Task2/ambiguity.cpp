// Task 2.2: what goes wrong without virtual inheritance
#include <iostream>
#include <string>
#ifdef NOVIRTUAL
#define V
#else
#define V virtual
#endif
struct Person { std::string name = "Asha"; };
struct Student : V Person {};
struct Staff   : V Person {};
struct TeachingAssistant : Student, Staff {};
int main() {
    TeachingAssistant ta;
    std::cout << ta.name << '\n';            // ambiguous without virtual
}
