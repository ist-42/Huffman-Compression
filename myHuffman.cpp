//
//  main.cpp
//  huffman
//
//  Created by Haoran Qin on 8/26/22.
//
#include <fstream>
#include <sstream>
#include <queue>
#include <iostream>
#include <cassert>
#include "myHuffman.h"
#include <string>

HuffManTree::HuffManTree() {
    encodeComplete = false;
    root = new HuffManTreeNode();
    origLength = 0;
    //initialize
    freqVector.resize(256, 0);
    codeDict.resize(256, "");
}

void HuffManTree::Encode(std::ifstream &inFile, std::ofstream &outFile, std::ofstream &treeFile) {
    assert(inFile.is_open());
    //build pq
    BuildPriorityQueue(inFile);

    //generate huffman code
    UpdateCode(pq);
    storeTreeFile(treeFile);
    std::string result = getEncodedString(inFile);
    outFile.write(result.c_str(), result.size());
    outFile.close();
}

void HuffManTree::BuildPriorityQueue(std::ifstream &inFile) {
    char inChar;
    while(inFile.peek()!=EOF) {
        inFile.get(inChar);
        freqVector[(int)inChar]++;
    }
    for (int c = 0; c < 256; c++) {
        if(freqVector[c]==0) continue;
        HuffNodePtr node = new HuffManTreeNode(c, freqVector[c]);
        pq.push(node);
    }
}

void HuffManTree::UpdateCode(std::priority_queue<HuffNodePtr, std::vector<HuffNodePtr>, LessThanByFreq> pq){
    //use an temporary priority queue clone to build huffman tree
    std::priority_queue<HuffNodePtr, std::vector<HuffNodePtr>, LessThanByFreq> pqCopy(pq);
    
    while (pqCopy.size()>1) {
        HuffNodePtr parent = new HuffManTreeNode();
        parent->l = pqCopy.top();
        pqCopy.pop();
        parent->r = pqCopy.top();
        pqCopy.pop();
        parent->freq = parent->l->freq + parent->r->freq;
        pqCopy.push(parent);
    }
    root = pqCopy.top();
    UpdateCodeRecur(root, "");
}

void HuffManTree::UpdateCodeRecur(const HuffNodePtr node, std::string hCode){
    if(!node->l && !node->r) {
        //leaf node, assign code accordingly
        node->code = hCode;
        codeDict[node->c] = hCode;
        return;
    }
    UpdateCodeRecur(node->l,  hCode + '0');
    UpdateCodeRecur(node->r,  hCode + '1');
}

void HuffManTree::storeTreeFile(std::ofstream& treeFile) {
    //use another temporary priority queue clone to store tree file
    std::priority_queue<HuffNodePtr, std::vector<HuffNodePtr>, LessThanByFreq> pqTree(pq);
    while(!pqTree.empty()) {
        HuffNodePtr node = pqTree.top();
        pqTree.pop();
        //format: [node char] [node huffmancode] [node freq]
        //one huffmannode each line, split by space
        treeFile << "h " << (int)node->c << ' ' << node->code << ' ' << node->freq << std::endl;
    }
    treeFile.close();
}

std::string HuffManTree::getEncodedString(std::ifstream& inFile) {
    std::string result = "";
    std::string dumpBuffer = "";
    char alphabetChar, binaryChar;
    //rewind inFile pointer
    inFile.clear();
    inFile.seekg(0);

    //Encode input file
    while(inFile.peek()!=EOF) {
        inFile.get(alphabetChar);
        dumpBuffer += codeDict[alphabetChar];
        if(dumpBuffer.size()>=8) {
            //TODO: use return value
            int ret = string2byte(dumpBuffer, binaryChar);
            if(ret) {
                std::cout << "Warning: binary char: " <<  binaryChar << std::endl;
            }
            result += binaryChar;
        }
    }
    inFile.close();

    //align tail byte
    size_t zerosForAlign = 8-dumpBuffer.size();
    if(dumpBuffer.size()>0) {
        dumpBuffer.append(zerosForAlign, '0');
    }
    int ret = string2byte(dumpBuffer, binaryChar);
    if(ret) {
        std::cout << "Warning: binary char: " <<  binaryChar << std::endl;
    }
    result += binaryChar;
    return result;
}

void HuffManTree::Decode(std::ifstream &encodedFile, std::ofstream &newFile, std::ifstream &treeFile) {
    restoreTree(treeFile);
    // decode compressed file
    std::string result = getDecodedString(encodedFile);
    newFile.write(result.c_str(), result.size());
    newFile.close();
}

void HuffManTree::restoreTree(std::ifstream &treeFile) {
    std::string line;
    //restore huffman tree
    while (treeFile.peek()!=EOF) {
        std::getline(treeFile, line);
        std::istringstream iss(line.c_str());
        char trash;
        if(!line.compare(0, 2, "h ")) {
            //huffman tree node format: 
            //  "h [char ascii code] [node huffmancode] [node freq]"
            iss >> trash;
            int symbol, freq;
            std::string code;
            iss >> symbol >> code >> freq;
            origLength += freq;
            HuffNodePtr temp = root;
            //navigate temp to its leaf node
            for(char c : code) {
                if(c=='0') {
                    if(!temp->l) {
                        temp->l = new HuffManTreeNode();
                    }
                    temp = temp->l;
                } else if (c=='1') {
                    if(!temp->r) {
                        temp->r = new HuffManTreeNode();
                    }
                    temp = temp->r;
                }
            }
            temp->c = (char)symbol;
            temp->code = code;
            codeDict[symbol] = code;
        }
    }
}

std::string HuffManTree::getDecodedString(std::ifstream &encodedFile) {
    char bits;
    std::string result;
    HuffNodePtr temp = root;
    while(encodedFile.peek()!=EOF) {
        encodedFile.get(bits);
        byte2string(bits, result, root, temp, origLength);
    }
    return result;
}
