//
//  Encoder.cpp
//  huffman
//
//  Created by Haoran Qin on 8/26/22.
//

#include "myHuffman.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: ./huffEncode fileName" << std::endl;
        exit(1);
    }
    
    std::string inname = argv[1];
    std::ifstream infile (inname);
    if(!infile.is_open()) {
        std::cout << "can't open file: " << inname << std::endl;
        exit(1);
    }
    
    std::ofstream outfile (inname + ".hufffile", std::ios::trunc|std::ios::out);
    std::ofstream treefile (inname + ".hufftree", std::ios::trunc|std::ios::out);

    HuffManTree* huffTree = new HuffManTree();
    
    huffTree->Encode(infile, outfile, treefile);

    std::cout << "Huffman encoding process complete!" << std::endl;
    std::cout << "Command for decoding: ./huffDecode " + inname << std::endl;
    return 0;
}
