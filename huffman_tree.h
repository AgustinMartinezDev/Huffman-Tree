#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_
#include <map>
#include <unordered_map>
#include <string>
#include <iostream>
class huffman_node{
	public: 
		huffman_node(){}; 
		huffman_node(const int weight); 
		std::string code=""; 
		char name; 
		int frequency = -1; 
		bool isLeaf = false; 
		bool isRoot = false; 
		huffman_node* left = nullptr; 
		huffman_node* right = nullptr; 
		bool operator<(const huffman_node &rhs) const;
		huffman_node &operator=(const huffman_node &rhs);
};
class huffman_tree {
public: 
	huffman_node* rootptr = nullptr;
	huffman_tree(const std::string &file_name);
	huffman_tree(){}; 
	~huffman_tree();	
	std::unordered_map<char, int> charFrequency;
	std::unordered_map<int, std::string> encoding; 
	void SetMap(huffman_node* head); 
	std::string get_character_code(char character) const;
	std::string encode(const std::string &file_name) const;
	std::string decode(const std::string &string_to_decode) const;	
};

#endif