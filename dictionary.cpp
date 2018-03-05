#include "dictionary.h"
#include <fstream>

unsigned int hash_function::operator()(const std::string &s) const
{
    return SuperFastHash(s.c_str(), s.length());
}

uint32_t hash_function::SuperFastHash(const char *data, int len) const
{
    uint32_t hash = len, tmp;
    int rem;

    if (len <= 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
            hash ^= hash << 16;
            hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
            hash += hash >> 11;
            break;
        case 2: hash += get16bits (data);
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        case 1: hash += (signed char)*data;
            hash ^= hash << 10;
            hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

Dictionary::Dictionary(string s)
{
    readFile(s);
}

void Dictionary::readFile(string s)
{
    ifstream inf(s);

    // if textfile can't be opened
    if (!inf)
        throw std::invalid_argument("Couldn't open " + s + "!");

    string line;

    if (inf.is_open())
    {
        // add new words line by line
        while ( getline (inf,line) )
            if (!search(line))
                insert(line);

        inf.close();
    }
}

set<string> Dictionary::checkSpell(string word)
{
    set<string> wordCorrections;

    // go through possible solutions
    transportAdjacentLetters(word, wordCorrections);
    deleteEachLetter(word, wordCorrections);
    replaceLetter(word, wordCorrections);
    insertLetter(word, wordCorrections);

    return wordCorrections;
}

void Dictionary::insertLetter(string s, set<string> &corrections)
{
    string word = s;

    for (int i = 0; i < s.length(); ++i, word = s)
        for (char ch = 'a'; ch < 'z'; ++ch, word = s)
        {
            word.insert(i, 1, ch);
            dictionaryCheck(word, corrections);
        }
}

void Dictionary::replaceLetter(string s, set<string> &corrections)
{
    string word = s;

    for (int i = 0; i < s.length(); ++i, word = s)
        for (char ch = 'a'; ch <= 'z'; ++ch, word = s)
        {
            word.insert(i, 1, ch);
            word.erase(i + 1, 1);
            dictionaryCheck(word, corrections);
        }
}

void Dictionary::deleteEachLetter(string s, set<string> &corrections)
{
    string word = s;

    for (int i = 0; i < s.length(); ++i, word = s)
    {
        word.erase(i, 1);
        dictionaryCheck(word, corrections);
    }
}

void Dictionary::transportAdjacentLetters(string s, set<string> &corrections)
{
    string word = s;

    if (s.length() == 1 && !search(s))
        return;

    for (int i = 1; i < s.length(); ++i, word = s)
    {
        word.insert(i - 1, 1, word[i]);
        word.erase(i + 1, 1);
        dictionaryCheck(word, corrections);
    }
}

void Dictionary::dictionaryCheck(string word, set<string> &corrections)
{
    if (search(word))
        corrections.insert(word);
}