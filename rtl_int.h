struct slot;

class rtl_int
{

public:

    rtl_int();
    rtl_int(const rtl_int & that);
    rtl_int(int value);
    rtl_int(const char string[]);
    ~rtl_int();


    rtl_int & operator=(const rtl_int & that)
    {

    }

    // Depending on the signs of LHS and RHS, either addition or subtraction is required.
    rtl_int & rtl_int::operator+=(const rtl_int & that)
    {
        if (isPositive && that.isPositive) 
        {
            add(that);
        } 
        else if (isPositive && !that.isPositive)
        {
            rtl_int placeholder(that);
            placeholder.isPositive = true;
            subtract(placeholder);
        }
        else if (!isPositive && that.isPositive)
        {
            rtl_int placeholder(that);
            placeholder.subtract(*this);
            copy(placeholder);
        }
        else
        {
            add(that);
            isPositive = false;
        }

        return *this;
    }

    rtl_int operator+(const rtl_int & that) const
    {
        return rtl_int(*this) += that;
    }

    // subtraction operators are defined as LHS - RHS = LHS + (-1 * RHS)
    rtl_int & operator-=(const rtl_int & that)
    {
        rtl_int placeholder(that);
        placeholder.isPositive = !placeholder.isPositive;
        return *this += placeholder;
    }

    rtl_int operator-(const rtl_int & that) const
    {
        return rtl_int(*this) -= that;
    }


    // Equality check, first check for same amount of slots. If that differs, numbers can't be equal. Second checks for signs, then checks if all slots themselves have equal value.
    bool operator==(const rtl_int & that) const
    {
        if (this->numberOfSlots != that.numberOfSlots)
        {
            return false;
        }
        if (isPositive != that.isPositive)
        {
            return false;
        }

        return true;
    }

    explicit operator bool() const
    {
        return *this != 0;
    }

    bool operator!() const
    {
        return !bool(*this);
    }

    friend std::ostream & operator<<(std::ostream & os, const rtl_int & obj)
    {
        if (obj != nullptr)
        {
            os.put(obj.binary_number);
        }
        return os;
    }

private:

    int binary_limit;
    std::string original_number;
    std::string binary_number;


    void init_number(const char* orig_number, short radix, int num_bits);
    void init_number(const char* orig_number, short radix);
    void init_number(const char* orig_number);

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
    unsigned long long odin_strtoull(const char *orig_number, short radix, int num_bits);
    unsigned long long odin_strtoull(const char *orig_number, short radix);

    /*******************************************************************
     *  convert to bitstring a number input and truncate to size
     * input
     *******************************************************************/
    const char *to_bitstring(unsigned long long number);
    const char *to_bitstring(unsigned long long number, int num_bits);
    const char *to_bitstring(const char *number, short radix);
    const char *to_bitstring(const char *number, short radix, int num_bits);

    bool containing_only_character_from_list(const char *string, const char *char_list);
    bool not_containing_character_from_list(const char *string, const char* char_list);

    bool is_dont_care_string(const char *string)		;
    bool is_string_of_radix(const char *string, short radix);

    const char *verilog_input_to_binary_string(const char *orig_string);

};