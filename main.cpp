#include <iostream>
#include <string>
#include <bitset>
#include <random>
#include <ctime>
// ********************************************************************
// ********************************************************************
// * Project: Simplified IDEA ALgorithm                               *
// * Description: This program is an enryption tool. It allows the    *
// *    user to input strings between 16 and                          *
// *    256 values. Then prompts the user for a 32 bit key to p[erform*
// *    the Simplified IDEA Algorithm. This program uses the bitset   *
// *    library to manipulate the data to do Modular arithmetic amd   *
// *    XOR operations.                                               *
// * Author: Donald Kremer                                            *
// * Date: 11-17-2023                                                 *
// * References: Sean Eron Anderson, Stanford University.             *
// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64*
// *    cppreference.com/w/cpp/utility/bitset                         *
// *    Nick Hoffman: A Simplified Idea Algorithm                     *
// ********************************************************************
// ********************************************************************
using namespace std;
// ********************************************************************
// ********************************************************************
// Method declarations                                                *
// ********************************************************************
// ********************************************************************
unsigned int Round1_4(unsigned int N1, unsigned int N2, unsigned int N3, unsigned int N4, unsigned int k);
// ********************************************************************
// ********************************************************************
unsigned int Round5(unsigned int N1, unsigned int N2, unsigned int N3, unsigned int N4, unsigned int k);
// ********************************************************************
// ********************************************************************
unsigned int CircularShift(unsigned int in, int nb);
// ********************************************************************
// ********************************************************************
unsigned int ModMultiplication(unsigned int N1, unsigned int N2);
// ********************************************************************
// ********************************************************************
unsigned int ModAddition(unsigned int N1, unsigned int N2);
// ********************************************************************
// ********************************************************************
unsigned int ModXOR(unsigned int N1, unsigned int N2);
bool validateKey(const string k);
// ********************************************************************
// ********************************************************************
// * Function: getRandomKey()                                         *
// * Description: Creates a randomly generated key and returns it     *
// *    as a string.                                                  *
// * Parameter: Unisigned integer instr and returns an unsigned int   *
// *    nb (number of bits).                                          *
// * Author: Donald Kremer                                            *
// * Date: 11-17-2023                                                 *
// * References: Sean Eron Anderson, Stanford University.             *
// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64*
// *    cppreference.com/w/cpp/utility/bitset                         *
// *    Nick Hoffman: A Simplified Idea Algorithm                     *
// ********************************************************************
// ********************************************************************
std::string getRandomKey(const int word)
{
    static const char isbool[] = {0,1};
    std::string temp;
    temp.reserve(word);
    // Fil out temp string with bit/boolean values
    for (int i = 0; i < word; ++i)
    {
        temp += isbool[rand() % 2];
    }
    return temp;
}
const int KEYSIZE = 32;
// ********************************************************************
// ********************************************************************
// * Function: main()                                                 *
// * Description: Main driver for the program. Takes two inputs from  *
// *    the user (one plain text, the other a key) and returns am     *
// *    an encrypted string using the Simplified IDEA algorithm.      *
// * Parameter: N/A                                                   *
// * Author: Donald Kremer                                            *
// * Date: 11-17-2023                                                 *
// * References: Sean Eron Anderson, Stanford University.             *
// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64*
// *    cppreference.com/w/cpp/utility/bitset                         *
// *    Nick Hoffman: A Simplified Idea Algorithm                     *
// ********************************************************************
// ********************************************************************
int main() {
    // Welcome the user
    std::cout << "Welcome to the Simplified IDEA Algorithm Encryption program." << std::endl;
    // Initialize random seed
    srand(time(NULL));
    // Initialize strings
    string instr;
    string key;
    cout << "Enter a string between 16 and 256 characters: ";
    getline(cin, instr);
    while (instr.length() < 16 || instr.length() > 256)
    {
        cout << "Error, enter in a number between 16 and 256: ";
        getline(cin, instr);
    }
    cout << endl;
    cout << "Enter in a 32-bit binary key (don't worry, we " << + "can make one if you cannot keep track): ";
    getline(cin, key);
    // Check for validation, if not generate random key
    if (!validateKey(key))
    {
        key = getRandomKey(KEYSIZE);
    }
    cout << "Pre-Encrypted text: "<< instr << endl;
    cout << "Key: " << key << endl;
    int i= 0;
    // Iterate through the entire length of the input string
    while( i < instr.length())
    {
        // Tokenize the plain text string into nibbles
        unsigned int X1 = instr[i];
        unsigned int X2 = instr[i + 1];
        unsigned int X3 = instr[i + 2];
        unsigned int X4 = instr[i + 3];
        // The 32-bit key is split into 8 4-bit blocks, which become the sub keys
        unsigned int Z1 = key[0];
        unsigned int Z2 = key[1];
        unsigned int Z3 = key[2];
        unsigned int Z4 = key[3];
        // Round 1
        unsigned int result = Round1_4(X1, X2, X3, X4, Z1);
        // Round 2
        result = Round1_4(result, X1, X2, X3, Z2);
        // Round 3
        result = Round1_4(result, X2, X3, X4, Z3);
        // Round 4
        result = Round1_4(result, X3, X4, X1, Z4);
        // Testing
        // cout << result << endl;

        //For every round except the final transformation, a swap occurs, and the input
        //to the next round is: result of step 11 k result of step 13 k result of step 12 k result
        //of step 14, which becomes X1 k X2 k X3 k X4, the input for the next round.
        result = Round5(result, X4, X1, X2, Z1);

        result = Round5(result, result, X4, X1, Z2);

        result = Round5(result, result, result, X4, Z3);

        result = Round5(result, result, result, result, Z4);
        // Concatenation of the blocks is the output
        cout << result << endl;
        // Cycle through to next nibble
        i += 4;
    }
    cout << endl;
    return 0;
}
// ********************************************************************
// ********************************************************************
// * End of main routine                                              *
// ********************************************************************
// ********************************************************************
// * Module Implementation                                            *
// ********************************************************************
// ********************************************************************

// ********************************************************************
// ********************************************************************
// * Function: CircularShift                                          *
// * Description: Shifts the bits of the unsigned int to the left     *
// *    using the << assignment overload from the bitset library.     *
// * Parameter: Unisigned integer instr and returns an unsigned int   *
// *    nb (number of bits).                                          *
// * Author: Donald Kremer                                            *
// * Date: 11-17-2023                                                 *
// * References: Sean Eron Anderson, Stanford University.             *
// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64*
// *    cppreference.com/w/cpp/utility/bitset                         *
// *    Nick Hoffman: A Simplified Idea Algorithm                     *
// ********************************************************************
// ********************************************************************
bool validateKey(const string k)
{
    // Temp variable to fill and test for length and
    //  whether contents are bits or not
    string temp = k;
    // Check string length accounting for index (0-31)
    if (k.length() == KEYSIZE - 1)
    {
        // Cycle through entire string
        for (int b = 0; b < temp.length(); b++)
        {
            // Check for bits
            if (temp[b] == 1 || temp[b] == 0)
            {
                return true;
            }
        }
    }
    return false;
}
// ********************************************************************
// ********************************************************************
// * Function: CircularShift                                          *
// * Description: Shifts the bits of the unsigned int to the left     *
// *    using the << assignment overload from the bitset library.     *
// * Parameter: Unisigned integer instr and returns an unsigned int   *
// *    nb (number of bits).                                          *
// * Author: Donald Kremer                                            *
// * Date: 11-17-2023                                                 *
// * References: Sean Eron Anderson, Stanford University.             *
// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64*
// *    cppreference.com/w/cpp/utility/bitset                         *
// *    Nick Hoffman: A Simplified Idea Algorithm                     *
// ********************************************************************
// ********************************************************************
unsigned int CircularShift(unsigned int instr, int nb){
    unsigned int result = instr;

    for (int index=0;index<1;index++)
    {
        // Bitset library performs binary AND, OR, XOR and NOT.
        // 1) & Sets the bits to the result of binary AND on corresponding pairs of bits of *this and other.
        // 2) | Sets the bits to the result of binary OR on corresponding pairs of bits of *this and other.
        // 3) ^ Sets the bits to the result of binary XOR on corresponding pairs of bits of *this and other.
        // 4) Returns a temporary copy of *this with all bits flipped (binary NOT).
        //Note that &=, |=, and ^= are only defined for bitsets of the same size N.
        unsigned int l_bit = result & 1;
        // Using the Bitset library to shift
        result = result >> 1;
        // or operator
        result = result | (l_bit << 31);
    }
    return result;
}
// ********************************************************************
// ********************************************************************
// * Function: Rounds1_4()                                            *
// * Description: Exectutes the first four rounds of the algorithm    *
// * Parameter: Four unsigned integers and the key                    *
// * Name: Donald Kremer                                              *
// * Date: 11-17-2023                                                 *
// * The fourteen steps of a complete round, for rounds 1-4.          *
//* 1. Multiply X1 and the first subkey Z1.                           *
//* 2. Add X2 and the second subkey Z2.                               *
//* 3. Add X3 and the third subkey Z3.                                *
//* 4. Multiply X4 and the fourth subkey Z4.                          *
//* 5. Bitwise XOR the results of steps 1 and 3.                      *
//* 6. Bitwise XOR the results of steps 2 and 4.                      *
//* 7. Multiply the result of step 5 and the fifth subkey Z5.         *
//* 8. Add the results of steps 6 and 7.                              *
//* 9. Multiply the result of step 8 and the sixth subkey Z6.         *
//* 10. Add the results of steps 7 and 9.                             *
//* 11. Bitwise XOR the results of steps 1 and 9.                     *
//* 12. Bitwise XOR the results of steps 3 and 9.                     *
//* 13. Bitwise XOR the results of steps 2 and 10.                    *
//* 14. Bitwise XOR the results of steps 4 and 10.                    *
// *    Swap occurs at end of every transformation.                   *
// * References: Sean Eron Anderson, Stanford University.             *
// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64*
// *    cppreference.com/w/cpp/utility/bitset                         *
// *    Nick Hoffman: A Simplified Idea Algorithm                     *
// ********************************************************************
// ********************************************************************
// ********************************************************************
// ********************************************************************
unsigned int Round1_4(unsigned int N1, unsigned int N2, unsigned int N3, unsigned int N4, unsigned int key)
{
    unsigned int result = ModMultiplication(N1, key);
    result = ModAddition(result, N2);
    result = ModAddition(result, N3);
    result = ModXOR(result, N4);
    return result;
}
// ********************************************************************
// ********************************************************************
// * Function: Round5()                                               *
// * Description: takes two unsigned integers and returns the modular *
// *    multiplicative result                                         *
// *    1. Multiply X1 and the first subkey.                          *
// *    2. Add X2 and the second subkey.                              *
// *    3. Add X3 and the third subkey.                               *
// *    4. Multiply X4 and the fourth subkey                          *
// *    The output is the concatenation of the results                *
// * Parameter: Two unsigned integers in1 and in2                     *
// * Name: Donald Kremer                                              *
// * Date: 11-17-2023                                                 *
// * References: Sean Eron Anderson, Stanford University.             *
// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64*
// *    cppreference.com/w/cpp/utility/bitset                         *
// *    Nick Hoffman: A Simplified Idea Algorithm                     *
// ********************************************************************
// ********************************************************************
unsigned int Round5(unsigned int N1, unsigned int N2, unsigned int N3, unsigned int N4, unsigned int k)
{
    unsigned int result = ModMultiplication(N1, k);
    result = ModXOR(result, N2);
    result = ModAddition(result, N3);
    result = ModAddition(result, N4);
    return result;
}
// ********************************************************************
// ********************************************************************
// * Function: ModMultiplication()                                    *
// * Description: takes two unsigned integers and returns the modular *
// *    multiplicative result                                         *
// * Parameter: Two unsigned integers in1 and in2                     *
// * Name: Donald Kremer                                              *
// * Date: 11-17-2023                                                 *
// * References: Sean Eron Anderson, Stanford University.             *
// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64*
// *    cppreference.com/w/cpp/utility/bitset                         *
// *    Nick Hoffman: A Simplified Idea Algorithm                     *
// ********************************************************************
// ********************************************************************
unsigned int ModMultiplication(unsigned int N1, unsigned int N2)
{
    unsigned int result = 0;
    unsigned int mask = N1;
    // Cycle through and multiply
    for (int i = 0; i < 32; i++)
    {
        if (N2 & 1)
        {
            // Modulo Addition with mask
            result = ModAddition(result, mask);
        }
        // Shift bits left
        mask = CircularShift(mask, 1);
        N2 = N2 >> 1;
    }
    // return result
    return result;
}
// ********************************************************************
// ********************************************************************
// * Function: ModAdd                                                 *
// * Description: Uses modular arithmetic and returns an unsigned int *
// * Parameter: Two unsigned inegers                                  *
// * Name: Donald Kremer                                              *
// * Date: 11-17-2023                                                 *
// * References: Sean Eron Anderson, Stanford University.             *
// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64*
// *    cppreference.com/w/cpp/utility/bitset                         *
// *    Nick Hoffman: A Simplified Idea Algorithm                     *
// ********************************************************************
// ********************************************************************
unsigned int ModAddition(unsigned int N1, unsigned int N2)
{
    // Modular addition
    unsigned int result = N1 + N2;
    // Bitset library comparison operator
    if (result < N1)
    {
        result++;
    }
    return result;
}
// ********************************************************************
// ********************************************************************
// * Function: ModXOR()                                               *
// * Description: Uses the XOR operations on two unsigned integers    *
// * Parameter: Two unsgined integers and returns a valuw             *
// * Name: Donald Kremer                                              *
// * Date: 11-17-2023                                                 *
// * References: Sean Eron Anderson, Stanford University.             *
// https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64*
// *    cppreference.com/w/cpp/utility/bitset                         *
// *    Nick Hoffman: A Simplified Idea Algorithm                     *
// ********************************************************************
// ********************************************************************
unsigned int ModXOR(unsigned int N1, unsigned int N2)
{
    // Bitset library XOR operation
    unsigned int result = N1 ^ N2;
    return result;
}
