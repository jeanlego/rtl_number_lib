#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>

#include "rtl_int.h"

#define UPPER_LIMIT 20

int main(int argc, char** argv) 
{ 
	if(1 < argc)
	{
		// std::cout << "Non-Interactive Mode:" << std::endl;

		// std::cout << "You have entered " << argc << " arguments:" << "\n";
		// for (int i = 0; i < argc; ++i)
		// {
		// 	std::cout << "argv[" << i << "]: " << argv[i] << "\n";
		// }

		if((4 == argc) && (0 == strcmp("1", argv[1])))
		{
			std::cout << "arithmetic(\"" << argv[2] << "\", \"" << argv[3] << "\")" << std::endl;

			std::string result = arithmetic(argv[2], argv[3]);

			std::cout << result << std::endl;
		}
		else if ((5 == argc) && (0 == strcmp("2", argv[1])))
		{
			std::cout << "arithmetic(\"" << argv[2] << "\", \"" << argv[3]<< "\", \"" << argv[4] << "\")" << std::endl;

			std::string result = arithmetic(argv[2], argv[3], argv[4]);

			std::cout << result << std::endl;
		}
		else if ((6 == argc) && (0 == strcmp("3", argv[1])))
		{
			std::cout << "arithmetic(\"" << argv[2] << "\", \"" << argv[3]<< "\", \"" << argv[4]<< "\", \"" << argv[5]<< "\", \"" << argv[6] << "\")" << std::endl;

			std::string result = arithmetic(argv[2], argv[3], argv[4], argv[5], argv[6]);

			std::cout << result << std::endl;
		}
		else
		{
			std::cout << "ERROR: Too Many Arguments: " << (argc - 1) << "!" << std::endl;
		}

		// std::cout << "Exiting..." << std::endl;
	}
	else
	{
		std::cout << "\nInteractive Mode:" << std::endl;
		//read input file space separated operation 1 / line
		for(std::size_t i = 0; i < UPPER_LIMIT; i++)
		{
			char typ[512];
			std::cout << "insert operation number of operand [1,2,3] (Q/q to quit): ";
			scanf("%s", typ);
			std::string comp = typ; 

			if (comp == "1")
			{
				char numb[2][512];
				scanf("%s %s", numb[0], numb[1]);
				std::string result = arithmetic(numb[0], numb[1]);
				std::cout << "Result is: " << result << std::endl;
			}
			else if (comp == "2")
			{
				char numb[3][512];
				scanf("%s %s %s", numb[0], numb[1], numb[2]);
				std::string result = arithmetic(numb[0], numb[1], numb[2]);
				std::cout << "Result is: " << result << std::endl;
			}
			else if (comp == "3")
			{
				char numb[5][512];
				scanf("%s %s %s %s %s", numb[0], numb[1], numb[2], numb[3], numb[4]);
				std::string result = arithmetic(numb[0], numb[1], numb[2], numb[3], numb[4]);
				std::cout << "Result is: " << result << std::endl;
			}
			else if (comp == "Q" || comp == "q")
			{
				i = (UPPER_LIMIT + 1);
				std::cout << "Exiting..." << std::endl;
			}
			else
			{
				std::cout << "ERROR wrong type" << std::endl;
			}
		}
	}

	return 0;
}
