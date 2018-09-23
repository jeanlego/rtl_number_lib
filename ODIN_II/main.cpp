#include <cstdio>
#include <string>
#include "rtl_int.h"
// #include "odin_ii.h"
// #include "types.h"
// #include "vtr_time.h"

void start_odin_ii();

int
main()
{
	// vtr::ScopedFinishTimer t("Odin II");
	start_odin_ii();
	// terminate_odin_ii();
	return 0;

}

void
start_odin_ii()
{
	printf("ODIN_II: main.cpp: start_odin_ii(): Init:\n");
	printf("ODIN_II: main.cpp: start_odin_ii(): std::string result = arithmetic(\"1\", \"+\", \"1\");\n");

	std::string result = arithmetic("1", "+", "1");

	printf("ODIN_II: main.cpp: start_odin_ii(): result: %s\n", result.c_str());
	printf("ODIN_II: main.cpp: start_odin_ii(): Done.\n");
	return;
}
