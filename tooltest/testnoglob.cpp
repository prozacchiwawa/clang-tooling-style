int g_yuck;

namespace AllowedCouplingNonAbstract {
    class TestClass { 
    public:
        int bang() { return 0; }
        int okey() const { return 0; }
    };
}

using AllowedCouplingNonAbstract::TestClass;

namespace AllowedCouplingAbstract {
    class EmptyAllowed {
    };

    class ITestAllowed {
    public:
        virtual ~ITestAllowed() { }
        virtual void Test() = 0;
    };

    class TestNotAllowed {
    public:
        void Test() { }
    };

    class TestNotAllowedDespiteVirtual {
        void NotOk() { }
        virtual void Test() { }
    };
}

namespace {
	const char *n_yuck = "no way";
	const char *const n_ok = "Hi there";
    char yuck_array[] = "yuck";
    const char ok_array[] = "yay";
}
extern TestClass *tc;
TestClass _tc;
static double gs_yuck = 0.333;
static const double gs_ok = 3.14159;
static constexpr float gs_ok_constexpr = 9.9999;

bool test(const char *data) {
    return !!data[0];
}

int f() {
    int dandy = 0;
	g_yuck = 10; // Matches assignment to global
    for (int i = 0; n_yuck[i]; i++) { // Reference to a mutable global
        g_yuck += n_yuck[i]; // mutate a global, reference to a mutable global
    }
    for (int i = 0; ok_array[i]; i++) {
        dandy += ok_array[i];
    }
    int *tehe_yuck = &g_yuck; // matches address of non-const global
    const double *tehe_ok = &gs_ok;
    n_yuck = "crud"; // matches assignment to global (but the pointer is to const elements)
    _tc.bang(); // non-const method call of global object
    tc->bang(); // non-const method call through global pointer
    _tc.okey();
    tc->okey();
	return 0;
}

int g() {
	static int s_yuck = 3;
	static const int s_init_ok = 99;
    if (yuck_array[0]++ == 99) {
        delete tc; // Should not delete data found in a global
    }
    test(ok_array);
    test(yuck_array);
	return s_yuck + gs_yuck + gs_ok + gs_ok_constexpr;
}

int main()
{
	f();
	return g();
}
