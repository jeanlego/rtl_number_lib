#include <string>

class rtl_int
{

private:
    int binary_limit;
    bool signed_int;
    std::string binary_number;
    std::string original_number;

 public:   
    //make duplicates
    rtl_int(const rtl_int & that);
    // preparsed input
    rtl_int(std::string orig_number, bool signed_int, int num_bits);
    // verilog type input
    rtl_int(std::string orig_number);
    // numeral type input
    rtl_int(unsigned long long, int num_bits);
    rtl_int(long long, int num_bits);

    /*** allowable operator overloading
     * 
     *
     

    Arithmetic
        + (addition)
        - (subtraction)
        * (multiplication)
        / (division)
        % (modulus)

    Bitwise
        ^ (XOR)
        | (OR)
        & (AND)
        ~ (Complement)
        << (Shift Left, Insertion to Stream)
        >> (Shift Right, Extraction from Stream)

    Assignment
        = (Assignment)

    Relational
        == (Equality)
        != (Inequality)
        > (Greater-Than)
        < (Less-Than)
        >= (Greater-Than Or Equal-To)
        <= (Less-Than Or Equal-To)

    Logical
        ! (NOT)
        && (AND)
        || (OR)

    Compound Assignment
        += (Addition-Assignment)
        -= (Subtraction-Assignment)
        *= (Multiplication-Assignment)
        /= (Division-Assignment)
        %= (Modulus-Assignment)
        &= (AND-Assignment)
        |= (OR-Assignment)
        ^= (XOR-Assignment)
        <<= (Shift-Left Assignment)
        >>= (Shift-Right Assignment)

    Increment - Decrement - Both have 2 forms (prefix) and (postfix)
        ++ (Increment)
        -- (Decrement)

    Subscript
        [] (Subscript)

    Function Call
        () (Function Call)

    Address, Reference, Pointer
        operator&()
        operator*()
        operator->()

    Comma
        operator,()

    Member Reference
        operator->()
        operator->*()

    Memory Management
        new
        delete
        new[]
        delete[]

    Conversion
        operator "type" () const

    NON Modifiable Operators - Operators that can not be overloaded
        ?: (Conditional - Ternary)
        . (Member Selection)
        .* (Member Selection With Pointer To Member)
        :: (Scope Resolution)
        sizeof() (Object Size Information)
        typeid() (Object Type Information)

     *  
     */

    rtl_int operator    ~   () const ;
    rtl_int operator    &   () const ;
    rtl_int operator    |   () const ;
    rtl_int operator    ^   () const ;
    rtl_int operator    ~&  () const ;

    // rtl_int & operator=(const rtl_int & that);

    // rtl_int & rtl_int::operator+=(const rtl_int & that)
    // {
    //     if (isPositive && that.isPositive) 
    //     {
    //         add(that);
    //     } 
    //     else if (isPositive && !that.isPositive)
    //     {
    //         rtl_int placeholder(that);
    //         placeholder.isPositive = true;
    //         subtract(placeholder);
    //     }
    //     else if (!isPositive && that.isPositive)
    //     {
    //         rtl_int placeholder(that);
    //         placeholder.subtract(*this);
    //         copy(placeholder);
    //     }
    //     else
    //     {
    //         add(that);
    //         isPositive = false;
    //     }

    //     return *this;
    // }

    // rtl_int operator+(const rtl_int & that) const
    // {  
    //     return rtl_int(*this) += that;
    // }

    // rtl_int & operator-=(const rtl_int & that)
    // {
    //     rtl_int placeholder(that);
    //     placeholder.isPositive = !placeholder.isPositive;
    //     return *this += placeholder;
    // }

    // rtl_int operator-(const rtl_int & that) const
    // {
    //     return rtl_int(*this) -= that;
    // }

    // bool operator==(const rtl_int & that) const
    // {
    //     if (this->numberOfSlots != that.numberOfSlots)
    //     {
    //         return false;
    //     }
    //     if (isPositive != that.isPositive)
    //     {
    //         return false;
    //     }

    //     return true;
    // }

    // explicit operator bool() const
    // {
    //     return *this != 0;
    // }

    // bool operator!() const
    // {
    //     return !bool(*this);
    // }

    friend std::ostream & operator<<(std::ostream & os, const rtl_int & obj)
    {
        if (obj != nullptr)
        {
            return std::string("\'b" + obj.binary_number);
            os.put(obj.binary_number);
        }
        return os;
    }
};