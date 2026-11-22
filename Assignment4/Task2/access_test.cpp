// Task 2.4: compile-time test of member visibility. Build with -DINH=public|protected|private
// and -DWHERE_OUTSIDE (access from main) or -DWHERE_INSIDE (access from a Student member function)
// and one of -DT_PUB -DT_PROT -DT_PRIV
class Person {
public:    int pub = 1;
protected: int prot = 2;
private:   int priv = 3;
};
class Student : INH Person {
public:
#ifdef WHERE_INSIDE
    int probe() {
#ifdef T_PUB
        return pub;
#elif defined(T_PROT)
        return prot;
#else
        return priv;
#endif
    }
#endif
};
int main() {
    Student s; (void)s;
#ifdef WHERE_OUTSIDE
#ifdef T_PUB
    return s.pub;
#elif defined(T_PROT)
    return s.prot;
#else
    return s.priv;
#endif
#else
    return s.probe();
#endif
}
