#ifndef  _DICTIONARY_H_ 
#define  _DICTIONARY_H_

#include  <iostream>
#include  <vector>
#include  <list>
#include  <algorithm>
#include  <string>
#include <set>

#include  "hashset.h"
#include  "hashset.cpp"

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

//using namespace std;

class hash_function {
public:
    hash_function() {}

    unsigned int operator()(const std::string& s) const;

    /**
     * Superfast hash function
     * @param data word for dictionary
     * @param len length of the word
     * @return hash
     */
    uint32_t SuperFastHash (const char * data, int len)const;
};

class equality {
public:
    equality() {}
    bool  operator()(const std::string& a, const std::string& b)  const 
    {
		return  (a == b);
    }
};


class Dictionary: public HashSet<std::string, hash_function, equality>
{
public:
    /**
     * constructor
     * @param s path to a file with data
     */
    Dictionary(string s);

    /**
     * Read text file to fill up the dictionary
     * @param s path to a file
     */
    void readFile(string s);

    /**
     * Check \c word spelling
     * @param word misspelled word
     * @return set of suggested corrections
     */
    set<string> checkSpell(string word);

    /**
     * Check if a \c word is in the dictionary
     * @param corrections if it is, add to \c corrections set
     */
    void dictionaryCheck(string word, set<string> &corrections);

    /**
     * Transporting adjacent letters
     */
    void transportAdjacentLetters(string s, set<string> &corrections);

    /**
     * Deleting each letter one by one
     */
    void deleteEachLetter(string s, set<string> &corrections);

    /**
     * Replacing each letter one by one
     */
    void replaceLetter(string s, set<string> &corrections);

    /**
     * inserting a letter in every place in a word \c s
     */
    void insertLetter(string s, set<string> &corrections);
};

#endif
