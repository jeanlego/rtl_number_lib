/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#include "VerilogBits.hpp"

int main(int argc, char **argv)
{
    size_t size=0;
    size = strtoul(argv[1],NULL,10);
	VerilogBits my_bits(size,'x');
    my_bits.print_type();
    printf("array_size(%zu) \n\n================\n", my_bits.size());

	my_bits.print();
	
	for(size_t value = 0; value <8; value++)
	{
		for(size_t i=0; i<size; i++)
		{
			printf("(%zu)[%zu] : ",value,i);
			my_bits.set_bit(i,value);
			my_bits.print();
		}
	}
}
