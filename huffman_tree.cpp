#include "huffman_tree.h"
#include <map>
#include <unordered_map>
#include <string>
#include <fstream> 
#include <iostream>
#include <queue>
#include <stack>
using namespace std; 

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Reads the contents of file_name and constructs a
				huffman tree based on the character frequencies of the file contents
*/

bool contains(const unordered_map<char, int> &m, char a){ // linear search through a map
	auto it = m.begin(); 
	for(; it!= m.end(); it++){
		if(it->first == a){
			return true; 
		}
	}
	return false; 
}

/*
does an in order traversal and when it hits a leaf nodee adds it and its code to a map
*/
void huffman_tree::SetMap(huffman_node* head){
    if(head != nullptr){
        SetMap(head->left);
		if(head->right == nullptr && head->left == nullptr){
        	encoding.emplace((int)head->name, head->code); 
		}
        SetMap(head->right);
    }
}

void SetCode(huffman_node* root){
	if(root->left != nullptr){ // go left 
		root->left->code = root->code+'0'; // set child code based on parent
		SetCode(root->left); 
	}
	if(root->right != nullptr){ // go right 
		root->right->code = root->code+'1'; // set child code based on parent
		SetCode(root->right); 
	}
	if(root->isRoot){root->code = '1';} //base 
}

void Delete(huffman_node* root){ // in order traversal to free memory 
	if(root == nullptr){
		return;
	}
	Delete(root->left); 
	Delete(root->right); 
	delete root;  
}

huffman_node::huffman_node(int weight){
	this->frequency = weight; 
}

bool huffman_node::operator<(const huffman_node &rhs) const{ // makes a priority queue default to a min priority queue 
	if(this->frequency < rhs.frequency){
		return false; 
	}
	return true; 
}

huffman_node &huffman_node::operator=(const huffman_node &rhs){ // assignment operator for deep copies 
	this->name = rhs.name; 
	this->frequency = rhs.frequency; 
	this->isLeaf = rhs.isLeaf; 
	this->left = rhs.left; 
	this->right = rhs.right; 
	return  *this; 
}

huffman_tree::huffman_tree(const std::string &file_name){ 
	priority_queue<huffman_node> MinPQ; 
	ifstream myfile; 
	myfile.open(file_name); 
	char carrier; 
	if(myfile.is_open()){ // read a text file 
		while(myfile.get(carrier)){
			if(contains(charFrequency,carrier)){ // check if it exists 
				charFrequency[carrier]++; 
			}
			else{ // new character 
				charFrequency.emplace(carrier, 1); 
			}
		}
	}
	auto it = charFrequency.begin(); 
	for(; it!= charFrequency.end(); it++){ // make the set of leaf nodes 
		huffman_node add(it->second); 
		add.name = it->first; 
		add.isLeaf = true; 
		MinPQ.push(add); // add leaves to the queue 
	}
	if(MinPQ.size() == 1){ // base case for a 1 node tree 
		huffman_node * rootNode = new huffman_node();
		*rootNode = MinPQ.top(); 
		rootNode->isRoot =true; 
		MinPQ.pop();
		this->rootptr = rootNode; 
	}
	else{
		while(MinPQ.size()!= 1){ // algorithm condition
			huffman_node * hold = new huffman_node(); 
			*hold = MinPQ.top(); 
			MinPQ.pop(); 
			huffman_node * hold2 = new huffman_node();
			*hold2 = MinPQ.top(); 
			MinPQ.pop();   
			huffman_node parent(hold->frequency+hold2->frequency); // making a parent node
			parent.left = hold;
			parent.right = hold2;  
			MinPQ.push(parent); // add parent of the two nodes that got popped off the queue into the queue 
		} 

		huffman_node * rootNode = new huffman_node();
		*rootNode = MinPQ.top(); // make the root of the tree equal to the remaining node in the queue 
		MinPQ.pop();
		this->rootptr = rootNode; 
	}
	SetCode(this->rootptr);
	SetMap(this->rootptr); 
}

huffman_tree::~huffman_tree(){
	Delete(rootptr); 
}

/*
Preconditions: Character is a character with an ASCII value
				between 0 and 127 (inclusive).
Postconditions: Returns the Huffman code for character if character is in the tree
				and an empty string otherwise.
*/

std::string huffman_tree::get_character_code(char character) const {
	auto it = encoding.begin(); 
	for(; it!= encoding.end(); it++){
		if(it->first == (int)character){
			return it->second; 
		} 
	}
	return ""; 
}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Returns the Huffman encoding for the contents of file_name
				if file name exists and an empty string otherwise.
				If the file contains letters not present in the huffman_tree,
				return an empty string
*/

std::string huffman_tree::encode(const std::string &file_name) const {
	string code; 
	ifstream myfile; 
	if(!myfile.good()){return "";}
	myfile.open(file_name); 
	char carrier; 
	if(myfile.is_open()){
		while(myfile.get(carrier)){
			if(!contains(charFrequency, carrier)){ // check validity 
				return ""; 
			}
			code+=get_character_code(carrier); // appends the code of a given character 
		}
	}
	return code;
}

/*
Preconditions: string_to_decode is a string containing Huffman-encoded text
Postconditions: Returns the plaintext represented by the string if the string
				is a valid Huffman encoding and an empty string otherwise
*/

std::string huffman_tree::decode(const std::string &string_to_decode) const {
	string text=""; 
	huffman_node* traverser = this->rootptr; 
	if(string_to_decode.size() == 0){
		return ""; 
	}
	for(unsigned int i=0; i<string_to_decode.length(); i++){
		if(string_to_decode.at(i) == '0' && traverser->left != nullptr){ //go left 
			traverser = traverser->left; 
		}
		else if(string_to_decode.at(i) == '1' && traverser->right != nullptr){ // go right 
			traverser = traverser->right; 
		}
		if (traverser->isLeaf){ // reset to the root 
			text += traverser->name; 
			traverser = rootptr;  
		}
		if(string_to_decode.at(i) != '1' && string_to_decode.at(i) != '0'){ // function condition 
			return ""; 
		}
	}
	return text;
}
