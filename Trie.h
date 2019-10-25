#ifndef TRIE_H
#define TRIE_H

#include "TrieNode.h"
#include <string>
#include <cctype>
using namespace std;

class Trie
{
private:
	TrieNode* _root = nullptr;

protected:

public:
	Trie()
	{
		_root = new TrieNode{};
	}

	virtual ~Trie()
	{
		//TODO: clean up memory
	}

	//TODO: implement
	void addWord(const string& word)
	{
		TrieNode* current = _root;
		for (auto character : word)
		{
			if (current->hasChild(character) == false)
			{
				current->setChild(character, new TrieNode(character));
			}
			current = current->getChild(character);
		}
		current->setChild('$', new TrieNode('$'));
	}

	//TODO: implement
	vector<string> search(const string& word)
	{
		vector<string> matches;
		return matches;
	}
};

#endif // !TRIE_H
