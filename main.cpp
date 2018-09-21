#include "stdio.h"
#include <iostream>
#include <string>
#include "rtl_int.h"
#define UPPER_LIMIT 20

int main()
{
	//read input file space separated operation 1 / line
	for(std::size_t i = 0; i<UPPER_LIMIT; i++)
	{
		char typ[512];
		std::cout << "\ninsert operation number of operand [1,2,3]: ";
		scanf("%s", typ);
		std::string comp = typ; 

		if (comp == "1") {
			char numb[2][512];
			scanf("%s %s", numb[0], numb[1]);
			std::cout << "Result is: " << arithmetic(numb[0],numb[1]);
		}else if (comp == "2") {
			char numb[3][512];
			scanf("%s %s %s", numb[0], numb[1], numb[2]);
			std::cout << "Result is: " << arithmetic(numb[0],numb[1],numb[2]);
		}else if (comp == "3") {
			char numb[5][512];
			scanf("%s %s %s %s %s", numb[0], numb[1], numb[2], numb[3], numb[4]);
			std::cout << "Result is: " << arithmetic(numb[0],numb[1],numb[2], numb[3], numb[4]);
		}else{
			std::cout << "ERROR wrong type";
		}
	}

	return 0;
}
