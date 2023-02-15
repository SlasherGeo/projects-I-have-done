//
// STARTER CODE: util.h
//
// NAME: Habeeb Rehman
// netID: mrehma22
// UIN: 676308443

#pragma once

#include <fstream>
#include <iostream>
#include "bitstream.h"
#include <unordered_map>
#include <queue>
#include <stdlib.h>

typedef hashmap hashmapF;
typedef unordered_map <int, string> hashmapE;

struct HuffmanNode {
    int character;
    int count;
    HuffmanNode* zero;
    HuffmanNode* one;
};

struct compare
{
    bool operator()(const HuffmanNode *lhs,
        const HuffmanNode *rhs)
    {
        return lhs->count > rhs->count;
    }
};

//
// *This method frees the memory allocated for the Huffman tree.
//
void freeTree(HuffmanNode* node) {
    if(node == nullptr){
        return;
    }

    freeTree(node->zero);
    freeTree(node->one);

    delete node;
}

//
// *This function build the frequency map.  If isFile is true, then it reads
// from filename.  If isFile is false, then it reads from a string filename.
//
void buildFrequencyMap(string filename, bool isFile, hashmapF &map) {
    ifstream inFS;
    if(isFile){
        inFS.open(filename);
        char sin;
        int charCount = 1;

        while (inFS.get(sin)) {
            if (map.containsKey(sin)) {
                int tempC = map.get(sin);
                tempC++;
                map.put(sin, tempC);
            } else {
                map.put(sin, charCount);
            }
        }
        inFS.close();
    }else{
        string expl = filename;
        int cCount = 1;
        for(char i : expl){
            if(map.containsKey(i)){

                map.put(i, map.get(i)+1);
            }else{
                map.put(i, cCount);
            }
        }
    }

    map.put(PSEUDO_EOF, 1);




    
}

//
// *This function builds an encoding tree from the frequency map.
//
HuffmanNode* buildEncodingTree(hashmapF &map) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, compare> pq;

    //get all the keys from hashmap
    vector<int> allKeys = map.keys();

    //going thru all keys, make a new node for each key to put into pq
    for(auto i : allKeys){
        auto *newNode = new HuffmanNode();
        newNode->character = i;
        newNode->count = map.get(i);
        pq.push(newNode);
    }



    //building the tree
    while(pq.size() > 1){
        //get first node on top
        HuffmanNode* firstP = pq.top();
        int firstFreq = firstP->count;
        pq.pop();
        //get second node on top
        HuffmanNode* secondP = pq.top();
        int secondFreq = secondP->count;
        pq.pop();
        //make a new node that has a count which is the sum of node1 and node2's freq count
        auto* newEnode = new HuffmanNode();
        newEnode->count = firstFreq + secondFreq;
        newEnode->character = NOT_A_CHAR;
        newEnode->zero = firstP;
        newEnode->one = secondP;

        pq.push(newEnode);
    }




    
    return pq.top();  // TO DO: update this return
}

//
// *Recursive helper function for building the encoding map.
//
void _buildEncodingMap(HuffmanNode* node, hashmapE &encodingMap, string str,
                       HuffmanNode* prev) {
    //once a char is reached, put it into the encoding map
    if(node->character != NOT_A_CHAR){
        encodingMap.emplace(node->character, str);
    }

    //look left, add a 0 to the string
    if(node->zero != nullptr){
        _buildEncodingMap(node->zero, encodingMap, str+"0", node);
    }
    //look right, add a 1 to the string
    if(node->one != nullptr){
        _buildEncodingMap(node->one, encodingMap, str+"1", node);
    }
    
}

//
// *This function builds the encoding map from an encoding tree.
//
hashmapE buildEncodingMap(HuffmanNode* tree) {
    hashmapE encodingMap;

    string path;

    //pass in pq node, along with the path string, map, and prev node (initialized as nullptr)
    _buildEncodingMap(tree, encodingMap, path, nullptr);

    
    return encodingMap;
}

//
// *This function encodes the data in the input stream into the output stream
// using the encodingMap.  This function calculates the number of bits
// written to the output stream and sets result to the size parameter, which is
// passed by reference.  This function also returns a string representation of
// the output file, which is particularly useful for testing.
//
string encode(ifstream& input, hashmapE &encodingMap, ofbitstream& output,
              int &size, bool makeFile) {

    //declare the returning string
    string encodedBits;

    //for each char in the file
    char encoder;
    while(input.get(encoder)){
        encodedBits += encodingMap[encoder]; //gets the appropriate bits from the encoding map then put into the string
    }
        encodedBits += encodingMap[PSEUDO_EOF]; //gets put at the end
    if(makeFile){
        for(char & encodedBit : encodedBits){
            int bit = int(encodedBit - '0'); //to get the actual int 0 or 1
            output.writeBit(bit);
            size++;
        }
    }
    
    return encodedBits;
}


//
// *This function decodes the input stream and writes the result to the output
// stream using the encodingTree.  This function also returns a string
// representation of the output file, which is particularly useful for testing.
//
string decode(ifbitstream &input, HuffmanNode* encodingTree, ofstream &output) {
    
    string decodedBits;
    HuffmanNode* root = encodingTree;
    HuffmanNode* curr = root;
    string debug;
    while(!input.eof()){
        int bit = input.readBit();
        debug += std::to_string(bit);

        if(curr->character != NOT_A_CHAR && curr->character != PSEUDO_EOF) {
            output << char(curr->character);
            decodedBits += char(curr->character);
            curr = root;
        }


        if(bit == 1){
            curr = curr->one;
            if(curr->character == PSEUDO_EOF){
                break;
            }
        }else if(bit == 0){
            curr = curr->zero;
            if(curr->character == PSEUDO_EOF){
                break;
            }
        }






    }



    return decodedBits;  // TO DO: update this return
}

//
// *This function completes the entire compression process.  Given a file,
// filename, this function (1) builds a frequency map; (2) builds an encoding
// tree; (3) builds an encoding map; (4) encodes the file (don't forget to
// include the frequency map in the header of the output file).  This function
// should create a compressed file named (filename + ".huf") and should also
// return a string version of the bit pattern.
//
string compress(string filename) {
    hashmap freqMap;
    ofbitstream bitFile(filename+".huf");
    ifstream input(filename);
    bool isFile = true;
    bool makeFile = true;
    int size = 0;

    buildFrequencyMap(filename, isFile, freqMap);
    bitFile << freqMap;
    bitFile << endl;
    HuffmanNode* encodeTree = buildEncodingTree(freqMap);
    hashmapE encodeMap = buildEncodingMap(encodeTree);

    string encodedS = encode(input, encodeMap, bitFile, size, makeFile);


    return encodedS;
}

//
// *This function completes the entire decompression process.  Given the file,
// filename (which should end with ".huf"), (1) extract the header and build
// the frequency map; (2) build an encoding tree from the frequency map; (3)
// using the encoding tree to decode the file.  This function should create a
// compressed file using the following convention.
// If filename = "example.txt.huf", then the uncompressed file should be named
// "example_unc.txt".  The function should return a string version of the
// uncompressed file.  Note: this function should reverse what the compress
// function did.
//
string decompress(string filename) {
    ifbitstream inputBits(filename);
    int indexIt = filename.find('.');
    string outFS = filename.substr(0, indexIt) + "_unc.txt";

    ofstream output(outFS);

    hashmap decodeMap;
    inputBits >> decodeMap;

    HuffmanNode* decodeTree = buildEncodingTree(decodeMap);
    string decoded = decode(inputBits, decodeTree, output);

    freeTree(decodeTree);
    return decoded;
}
