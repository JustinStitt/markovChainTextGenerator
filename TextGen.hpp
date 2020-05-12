/*
	PROJECT 3
	Justin Stitt
	last update: 5/2/2020
	
	(4/12) - Initial Creation
	(5/2) - Cleaned up readIn function with more readable code
*/
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <iostream>
#include "sanitize.hpp"

using namespace  std;

class TextGen {
private:
	map<string, vector<string>> markov_chain;
	string  delimiters = ".?!";//end punctuation, value should be '$'
public:
	TextGen(const unsigned int seed);//rand() is different across platforms. :(

	void readIn(const string & filename);
	bool isEndPunctuation(const char c);
	vector<string> getValues(const string & key);
	string generateSentence();
	void print_chain();
	void update_chain(ifstream& read, string& w1, string& w2);

	map<string, vector<string>> get_markovChain()
	{
		return this->markov_chain;
	}
};

string TextGen::generateSentence()
{
	string sentence = "";//blank sentence to start
	string key = "^";//always start new sentence generation with the ^ key
	int _r;
	string word;
	while (markov_chain.count(key) >= 1 && markov_chain[key][0] != "$")
	{
		_r = rand() % markov_chain[key].size();
		word = markov_chain[key][_r];
		if (isEndPunctuation(word[0]))
		{
			sentence.erase(sentence.size() - 1);
			sentence += word;
		}
		else sentence += (word + " ");
		key = word;
	}
	return sentence;
}

void TextGen::readIn(const string& filename)
{
	ifstream read; read.open(filename);
	string word = "^", next_word;

	while (read)
	{
		do { read >> next_word; sanitize(next_word); } while (next_word.empty());
		//right-end behaviour (if our next_word ends in punctuation)
		if (isEndPunctuation(next_word[next_word.size() - 1]))//if our next_word ends in punctuation
		{
			string cleaned_word = next_word.substr(0, next_word.size() - 1);
			markov_chain[word].push_back(cleaned_word);
			markov_chain[cleaned_word].push_back(string{ next_word[next_word.size() - 1] });
		}
		//left-end behaviour (if our current word ends in punctuation
		else if (isEndPunctuation(word[word.size() - 1]))
		{
			string cleaned_word = word.substr(0, word.size() - 1);
			markov_chain["^"].push_back(next_word);
		}
		//all other cases (middle of sentence behaviour)
		else
			markov_chain[word].push_back(next_word);
		word = next_word;
	}

}

vector<string> TextGen::getValues(const string& key)
{
	return this->markov_chain[key];
}

TextGen::TextGen(const unsigned int seed)
{
	srand(seed);
	//assign puncutation key with value '$'
	for (char& p : delimiters)
	{
		this->markov_chain[string{ p }].push_back("$");
	}
}

bool TextGen::isEndPunctuation(const char c)
{
	for (char& d : delimiters)
	{
		if (c == d)
			return true;
	}
	return false;
}
/*
	resources used:
	https://en.cppreference.com/w/cpp/container/map
	https://en.cppreference.com/w/cpp/utility/pair
	https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/
*/