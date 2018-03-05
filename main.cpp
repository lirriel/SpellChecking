#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <set>

#include "dictionary.h"

using namespace std;

void lower(string& s);
string stripPunct(const string& s);
void checkSpelling(ifstream& in, Dictionary& dict);


int main(int argc, char* argv[]) 
{
	// Output usage message if improper command line args were given.
	if (argc != 3)
    {
		cerr << "Usage: " << argv[0] << " wordlist_filename input_file\n";
		return EXIT_FAILURE;
	}

	ifstream inf(argv[2]);
	if (! inf) 
    {
		cerr << "Could not open " << argv[2] << "\n";
		return EXIT_FAILURE;
	}

	// Read dictionary, but let user know what we are working on.
	cout << "Loading dictionary, this may take awhile...\n";

	Dictionary d(argv[1]);

	checkSpelling(inf, d);

	inf.close();

	return EXIT_SUCCESS;
}


/**
 * Method to check words spelling using dictionary
 * @param word misspelled word
 * @param dict data base to use
 */
void checkWords(string word, Dictionary& dict)
{
    // get list of possible corrections
	set<string> wordCorrections = dict.checkSpell(word);

    cout << "\t\tsuggestions:" << endl;
	for (set<string>::iterator it = wordCorrections.begin(); it != wordCorrections.end(); ++it)
		cout << "\t\t\t" << (*it) << endl;
}


/**
 * edit word before checking
 */
void formatWord(string& word)
{
    word = stripPunct(word);
    lower(word);
}


void checkSpelling(ifstream& in, Dictionary& dict)
{
	int line_number = 0;

	while (in) 
    {
		++line_number;

		string line;
		getline(in, line);

  	    stringstream ss (stringstream::in | stringstream::out);
		ss << line;

		string word;
		while (ss >> word) 
        {
            // edit word
			formatWord(word);

            // check correctness
			if (dict.search(word))
				continue;

			cout << "line " << line_number << ":'" << word << "'" << endl;
            // output possible suggestions
			checkWords(word, dict);
		}
	}
}

void lower(string& s) 
{
    // Ensures that a word is lowercase
	for (int i = 0; i < s.length(); ++i) 
    {
		s[i] = tolower(s[i]);
	}
}

string stripPunct(const string& s) 
{
	// Remove any single trailing
	// punctuation character from a word.  
	if (ispunct(s[s.length() - 1])) 
		return s.substr (0, s.length() - 1);
	
    return s;
}
