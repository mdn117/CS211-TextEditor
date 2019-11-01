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
		TrieNode* current = _root;
		string temp_word = word;
		for (auto character : word)
		{
			if (current->hasChild(character))
			{
				current = current->getChild(character);
			}
			else
			{
				return matches;
			}
		}
		for (auto child : current->getChildren())
		{
			if (child.first != '$')
			{
				temp_word = temp_word + child.first;
				search_helper(matches, word + child.first);
			}
			else
			{
				matches.push_back(temp_word);
			}
		}

		return matches;
	}
	void search_helper(vector<string>& matches, const string& word)
	{
		TrieNode* current = _root;
		string temp_word = word;
		for (auto character : word)
		{
			if (current->hasChild(character))
			{
				current = current->getChild(character);
			}
			else
			{
				return;
			}
		}
		for (auto child : current->getChildren())
		{
			if (child.first != '$')
			{
				temp_word = temp_word + child.first;
				search_helper(matches, word + child.first);
			}
			else
			{
				matches.push_back(temp_word);
			}
		}
	}
};

#endif // !TRIE_H
