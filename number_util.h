
typedef enum
{
	DEC = 10,
	HEX = 16,
	OCT = 8,
	BIN = 2,
	LONG_LONG = 0
} bases;

typedef struct number_t_t
{
    int binary_limit;
    std::string original_number;
    bases base;

    std::string binary_number;

    short is_full; //'bx means all of the wire get 'x'(dont care)

    void init_number(const char* orig_number, bases base, int num_bits)
    {
        original_number = orig_number;
        binary_limit = num_bits;
        base = base;
        binary_number = to_bitstring(orig_number, base, num_bits);
        is_full=0;
    }

    void init_number(const char* orig_number, bases base)
    {

    }

    void init_number(const char* orig_number)
    {

    }


	long long getvalue()
    {
        return std::strtoll(binary_number.c_str(),NULL,2);
    }
	void setvalue(long long input)
} number_t;


/*---------------------------------------------------------------------------------------------
 * (function: twos_complement)
 * Changes a bit string to its twos complement value
 *-------------------------------------------------------------------------------------------*/
char *twos_complement(char *str);

/******************************************************************
 *  convert to unsigned long long a number input and truncate to size
 * input
 *****************************************************************/
unsigned long long odin_strtoull(unsigned long long orig_number, int num_bits);
unsigned long long odin_strtoull(const char *orig_number, int radix, int num_bits);
unsigned long long odin_strtoull(const char *orig_number, bases base, int num_bits);
unsigned long long odin_strtoull(const char *orig_number, int radix);
unsigned long long odin_strtoull(const char *orig_number, bases base);

/*******************************************************************
 *  convert to bitstring a number input and truncate to size
 * input
 *******************************************************************/
const char *to_bitstring(unsigned long long number);
const char *to_bitstring(unsigned long long number, int num_bits);
const char *to_bitstring(const char *number, int radix);
const char *to_bitstring(const char *number, bases base);
const char *to_bitstring(const char *number, int radix, int num_bits);
const char *to_bitstring(const char *number, bases base, int num_bits);

bool containing_only_character_from_list(const char *string, const char *char_list);
bool not_containing_character_from_list(const char *string, const char* char_list);

bool is_dont_care_string(const char *string)		;
bool is_string_of_base(const char *string, bases base);
bool is_string_of_radix(const char *string, int radix);

const char *verilog_input_to_binary_string(const char *orig_string);