#include "../lich/lua.hpp"
#include "../lich/program.h"
#include "../lich/ref.h"
#include "must.h"

using namespace std;

int main()
{
	extern void shared_ptr_test();
	shared_ptr_test();

	extern void containers_test();
	containers_test();

	extern void program_test();
	program_test();

	extern void ref_test();
	ref_test();

	extern void pcall_test();
	pcall_test();

	cout << "OK" << endl;
    return 0;
}

