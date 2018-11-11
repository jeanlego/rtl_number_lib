/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#include <string>
#include "rtl_utils.h"

class v_number 
{
public:

    //initialize using an msb to lsb bitstring
    v_number(std::string verilog_string);
    std::string to_v_string();

    //copy another number over
    v_number(v_number& other);

    void value(INT_TYPE);

    INT_TYPE get_value();
    void set_value(INT_TYPE input);

    //get a single bit from bitstring 
    char bits_from_msb(int index);
    char bits_from_lsb(int index);

    bool is_negative();
    char get_padding_bit();
    void resize(int len);

private:

    bool sign;
    std::string bitstring;
    INT_TYPE length;
};
