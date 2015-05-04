int g_yuck;
namespace {
	const char *n_yuck = "no way";
	const char *const n_ok = "Hi there";
}
static double gs_yuck = 0.333;
static const double gs_ok = 3.14159;
static constexpr float gs_ok_constexpr = 9.9999;

int f() {
    int dandy = 0;
	g_yuck = 10; // Matches assignment to global
    for (int i = 0; n_yuck[i]; i++) {
        g_yuck += n_yuck[i];
    }
    int *tehe_yuck = &g_yuck; // matches address of non-const global
    const double *tehe_ok = &gs_ok;
    n_yuck = "crud"; // matches assignment to global (but the pointer is to const elements)
	return 0;
}

int g() {
	static int s_yuck = 3;
	static const int s_init_ok = 99;
	return s_yuck + gs_yuck + gs_ok + gs_ok_constexpr;
}

int main()
{
	f();
	return g();
}
